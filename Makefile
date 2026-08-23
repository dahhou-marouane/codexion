NAME = codexion
HEADER = codexione/codexion.h codexione/struct.h

CFLAGS = -Wall -Wextra -Werror -pthread


FILES = codexione/coder.c codexione/dongle2.c codexione/free_all.c codexione/heap2.c \
		codexione/init.c codexione/monitor.c codexione/parsing.c codexione/sim.c \
		codexione/thread_mutex_cond.c codexione/dongle.c codexione/garbage_collector.c \
		codexione/heap.c codexione/main.c codexione/parsing2.c codexione/print.c codexione/time.c

OBJ = $(FILES:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	cc $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c $(HEADER)
	cc $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
