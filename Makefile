NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror -I.

LDFLAGS = -lreadline

SRCS =	minishell.c \
		init_data.c \
		free.c \
		args_before.c \
		args_after.c \
		builtins/builtins1.c \
		builtins/builtins2.c \
		builtins/builtins_helper1.c \
		builtins/builtins_helper2.c \
		builtins/builtins_helper3.c \
		libft/helper1.c \
		libft/helper2.c \
		libft/helper3.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re