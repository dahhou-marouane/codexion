*This project has been created as part of the 42 curriculum by mdahhou.*

# Codexion

## Description

Codexion is a concurrency simulation inspired by the Dining Philosophers problem. A group of coders sit in a circle, each sharing two dongles (USB devices) with their neighbors. To compile, a coder must acquire both dongles (left, right) simultaneously. Between compilations, coders debug and refactor. If a coder goes too long without compiling, they burn out and the simulation ends.

The project implements two scheduling algorithms for dongle allocation:

- **EDF (Earliest Deadline First):** priority is given to the coder closest to burning out, computed as `last_compile_start + time_to_burnout`. This minimizes starvation under tight timing constraints.
- **FIFO (First In, First Out):** priority is determined by a global sequence counter, serving coders in arrival order.

Priority is enforced through per-dongle min-heaps. Each dongle maintains a heap of waiting requests, and a coder can only acquire a dongle pair when it is the highest-priority waiter (peek) of both heaps simultaneously.

The simulation is multithreaded: each coder runs in its own pthread, coordinated by a monitor thread that detects burnout and completion.

## Instructions

### Compilation

```
make        # build
make clean  # remove object files
make fclean # remove object files and binary
make re     # full rebuild
```

Requires `cc` with pthread support. No external libraries.

### Execution

```
./Codexion <nb_coders> <t_burnout> <t_compile> <t_debug> <t_refactor> <nb_compiles> <dongle_cooldown> <scheduler>
```

All time values are in milliseconds. `scheduler` is either `fifo` or `edf`.

**Example — 5 coders, EDF scheduling, no one should burn out:**

```
./Codexion 5 800 200 100 100 7 0 edf
```

**Single coder (edge case — only one dongle, cannot compile):**

```
./Codexion 1 400 200 100 100 5 0 edf
```

### Validation

```
# Check for data races
valgrind --tool=helgrind ./Codexion 5 800 200 100 100 7 0 edf

# Check for memory leaks
valgrind --leak-check=full ./Codexion 5 800 200 100 100 7 0 edf
```

## Blocking cases handled

### Deadlock prevention (Coffman's conditions)

Deadlock requires four conditions to hold simultaneously: mutual exclusion, hold-and-wait, no preemption, and circular wait. Codexion breaks **circular wait** through a global lock ordering strategy: dongles are always locked in ascending order of their ID (`ft_lock_dongles`). Coder *i* uses dongles *i* and *(i+1) % n*; regardless of which is "left" or "right," the lower-ID dongle is always locked first. This total ordering on lock acquisition makes circular wait impossible.

### Starvation prevention

Each dongle maintains a min-heap of pending requests. Under EDF, the key is `last_compile_start + time_to_burnout` — the coder closest to burning out has the smallest key and wins the dongle. Under FIFO, the key is increasing global sequence number.

A coder must be the peek (highest priority) of **both** its dongles heaps to acquire them. This creates a potential problem: if a coder is peek of one heap but not the other, it blocks that dongle for everyone without being able to use it. Codexion handles this with (`ft_reaquire_dongles`): when a coder detects it is peek of exactly one heap, it removes itself from both heaps, sleeps, and re-inserts with a fresh priority key. This breaks the blocking chain while preserving EDF ordering for coders that are not causing contention.

### Dongle cooldown

After a coder releases a dongle pair, a cooldown period is enforced (`cooldown_end` timestamp per dongle). The `could_take_dongles` check verifies `now >= cooldown_end` before granting access. This prevents a single coder from monopolizing a dongle by immediately reacquiring it after release.

### Precise burnout detection

The monitor thread checks each coder's `l_com_start` timestamp against the current time. If `now - l_com_start > t_to_burnout`, the coder has burned out. To avoid false burnouts from thread creation overhead, all `l_com_start` values are initialized atomically to `start_time_mic` immediately before the barrier broadcast — ensuring the burnout clock starts at the same instant for every coder.

### Single-coder edge case

When `nb_coders == 1`, the coder's left and right dongles are the same physical dongle. Attempting to lock it twice would self-deadlock. Codexion detects `left == right` at thread start and enters a wait loop without attempting acquisition.

### Log serialization

All log output passes through `ft_print_logs`, which holds `log_lock` for the entire printf call. This guarantees that log lines from different threads never interleave. The "has taken a dongle" (×2) and "is compiling" messages for a single acquisition are printed in a single `printf` call under the lock, ensuring they share the same timestamp and appear as a group.

## Thread synchronization mechanisms

### Mutexes (`pthread_mutex_t`)

Codexion uses mutexes to protect every piece of shared mutable state:

- **Per-dongle locks** (`t_dongle.lock`): protect `taken`, `cooldown_end`, and the dongle's heap. Two dongle locks are always acquired together in ID order to prevent deadlock. All heap operations (push, peek, remove) happen while both dongle locked.
- **Per-coder lock** (`t_coder.lock`): protects `l_com_start` and `compile_count`. The monitor reads `l_com_start` under this lock; the coder writes it under this lock during `ft_compile`.
- **`log_lock`**: serializes all output. Held for the duration of each printf call.
- **`stop_lock`**: protects the `stop` flag. Read by every thread via `ft_sim_is_stoped`, written once by `ft_set_sim_stop`.
- **`f_routine_lock`**: protects the `f_routine` counter (number of coders that completed all required compilations). Written by coders, read by the monitor.
- **`seq_lock`**: protects the global FIFO sequence counter. Only used in FIFO mode.

**Race condition prevention example — burnout detection:**

```
// Monitor thread reads:            // Coder thread writes:
pthread_mutex_lock(coder->lock);    pthread_mutex_lock(coder->lock);
last = coder->l_com_start;          coder->l_com_start = ft_get_curr_t_mic();
pthread_mutex_unlock(coder->lock);  pthread_mutex_unlock(coder->lock);
```

Without `coder->lock`, the monitor could read a partially-written `long` value (torn read on 32-bit architectures) or see a stale value due to CPU cache effects, causing false burnout detection.

### Condition variable (`pthread_cond_t`)

A single condition variable (`start_cond` + `start_lock`) implements a start barrier. All coder threads and the monitor thread wait on this condvar after creation:

```
pthread_mutex_lock(Codexion->start_lock);
while (!Codexion->start)
    pthread_cond_wait(Codexion->start_cond, Codexion->start_lock);
pthread_mutex_unlock(Codexion->start_lock);
```

The main thread sets `l_com_start` for all coders, sets `start = 1`, and calls `pthread_cond_broadcast`. This ensures all threads begin executing at the same wall-clock instant, preventing early starters from gaining an unfair advantage and ensuring the burnout timer is synchronized.

The `while (!Codexion->start)` loop guards against spurious wakeups — a requirement of the POSIX condvar specification where `pthread_cond_wait` may return without a corresponding signal.

### Thread-safe communication between coders and monitor

The monitor and coder threads communicate through three shared variables, each protected by its own mutex:

1. **`stop` flag** (`stop_lock`): the monitor sets this on burnout or completion; all coder threads check it in their main loops and in `ft_usleep` to exit.
2. **`l_com_start`** (`coder->lock`): each coder updates this at the start of compilation; the monitor reads it to compute elapsed time since last compile.
3. **`f_routine`** (`f_routine_lock`): each coder increments this, reaching `nb_of_compiles_req`; the monitor checks if all coders have finished.

This design avoids a single global lock — each piece of state has its own mutex, minimizing contention. The monitor checks at 300μs intervals via `ft_usleep`, which itself checks the `stop` flag on each iteration, ensuring the monitor exits within ~500μs of a stop signal.

## Resources

- [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [POSIX Threads Programming](https://hpc-tutorials.llnl.gov/posix/)
- `man pthread_create`, `man pthread_mutex_lock`, `man pthread_cond_wait`, `man gettimeofday`
- [Earliest Deadline First Scheduling](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [Min-Heap](https://www.geeksforgeeks.org/dsa/introduction-to-min-heap-data-structure/)

### AI usage

AI (Claude by Anthropic) was used as a debugging and architectural review partner throughout the project. Specifically:

- **Bug diagnosis:** tracing starvation scenarios.
- **Theory:** explanations of pthread internals (fast/slow paths, `pthread_cond_wait`), and scheduler behavior.
- **Code review:** identifying dead code, unused struct fields, missing prototypes, and heap overflow risks.

All code was written and integrated by me. AI was not used to generate code ready to use.
