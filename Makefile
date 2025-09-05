NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror -I.

LDFLAGS = -lreadline

SRCS =	minishell.c \
		executor.c \
		expand_args1.c \
		expand_args2.c \
		fill_command.c \
		free.c \
		get_args.c \
		init_data.c \
		line_utils.c \
		builtins/cd_plus.c \
		builtins/echo_pwd_cd_export.c \
		builtins/export_plus.c \
		builtins/export_print.c \
		builtins/unset_env_exit.c \
		builtins/unset_plus.c \
		builtins/update_env.c \
		libft/func1.c \
		libft/func2.c \
		libft/func3.c \
		libft/func4.c \
		libft/func5.c

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