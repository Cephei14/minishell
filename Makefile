# The name of your final program
NAME = minishell

# Your C compiler
CC = gcc

# Compilation flags required by the subject.
# The -I. flag tells the compiler to look for header files in the current directory.
CFLAGS = -Wall -Wextra -Werror -I.

# The readline library flags, used during linking
LDFLAGS = -lreadline

# --- List of all your source files ---
# Manually list every .c file, one per line, with a backslash \ at the end
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

# --- Automatic Object File List ---
# This line automatically creates a list of .o files from your SRCS list
OBJS = $(SRCS:.c=.o)

# --- Makefile Rules ---

# Default rule: compile the program
all: $(NAME)

# Linking step: create the final executable from all the .o files
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

# Compilation step: rule to compile any .c file into a .o file
# This rule is called for each file in the OBJS list.
%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to remove temporary object files
clean:
	@rm -f $(OBJS)

# Rule to remove all compiled files, including the final program
fclean: clean
	@rm -f $(NAME)

# Rule to re-compile everything from scratch
re: fclean all

# This tells make that these rules don't create actual files
.PHONY: all clean fclean re