NAME = codexion
HEADER = codexion.h

CFLAGS = -Wall -Wextra -Werror -pthread


FILES = free_all.c init.c parsing.c print.c sim.c thread_mutex_cond.c \
		garbage_collector.c heap.c main.c routine.c time.c

OBJ = $(FILES:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	cc $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c $(HEADER)
	cc -g -O0 $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
