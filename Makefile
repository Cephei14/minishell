NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror -I.

LDFLAGS = -lreadline

SRCS =	minishell.c \
		init_data.c \
		free.c \
		args_before.c \
		args_after.c \
		builtins/cd_plus.c \
		builtins/echo_pwd_cd_export.c \
		builtins/export_plus.c \
		builtins/export_print_sorted_env.c \
		builtins/unset_env_exit.c \
		builtins/update_env.c \
		libft/func1.c \
		libft/func2.c \
		libft/func3.c

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