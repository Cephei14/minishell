/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 23:04:44 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/08 17:58:22 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L

# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

struct							s_data;
struct							s_built_in;
struct							s_command;
extern volatile sig_atomic_t	g_signal_received;

typedef struct s_data
{
	int					parse_error;
	int					last_exit_status;
	char				**args;
	char				**envp;
}	t_data;

typedef struct s_parse_state
{
	int	i;
	int	j;
}	t_state;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
	struct s_redir	*next;
}	t_redir;

typedef struct s_pipe_fds
{
	int	in_fd;
	int	pipe_fd[2];
}	t_pipe_fds;

typedef struct s_command
{
	char				**args;
	t_redir				*redirs;
	struct s_command	*next;
}	t_command;

typedef int						(*t_builtin_func)(t_data *data,
									t_command *command);

typedef struct s_built_in
{
	char			*cmds;
	t_builtin_func	func;
}	t_built_in;

/*
** Prototypes
*/

/* libft */
long long	ft_atoi(const char *str);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
char		*ft_itoa(int n);
void		ft_putstr_fd(char *s, int fd);
char		**ft_split(char const *s, char c);
char		*ft_strchr(const char *s, int c);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strdup(const char *s);
char		*ft_strjoin(char const *s1, char const *s2);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t		ft_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strtrim(char const *s1, char const *set);
char		*ft_substr(char const *s, unsigned int start, size_t len);
int			is_numeric(char *str);

/* builtins */
int			echo(t_data *data, t_command *command);
int			pwd(t_data *data, t_command *command);
int			cd(t_data *data, t_command *command);
int			export(t_data *data, t_command *command);
int			unset(t_data *data, t_command *command);
int			env(t_data *data, t_command *command);
int			b_exit(t_data *data, t_command *command);

/* builtins utils */
int			is_n_flag(const char *arg);
char		*get_cd_path(t_data *data, t_command *command);
int			cd_error(char *path, char *old_pwd);
void		update_pwd_vars(t_data *data, char *old_pwd);
int			is_valid_identifier(char *name);
char		*get_value(char *str, int i);
char		*get_name(char *str, int *i);
void		print_env(t_data *data);
void		handle_export_arg(t_data *data, t_command *command, char *arg);
void		remove_var_from_env(t_data *data, int index_to_remove, int count);
void		add_env_var(t_data *data, char *new_var_str);
void		set_env_variable(t_data *data, char *var_name, char *value);

/* parsing */
void		get_args(t_data *data, char *line);
void		remove_empty_args(t_data *data, int i, int j);
void		fill_commands(t_data *data, t_command *command, int i);
int			parse_one_command(t_data *data, t_command *cmd, int i);
int			count_cmd_args(char **args, int start);
int			is_operator(char *arg);
t_redir		*new_redir(char *filename, t_redir_type type);

/* expansion */
void		manage_env(t_data *data);
char		*build_clean_arg(t_data *data, char *arg, char quote_char);
void		handle_expansion(t_data *d, char *n_s, char *arg, t_state *st);
int			calculate_final_len(t_data *data, char *arg);
int			get_expanded_len(t_data *data, char *str, int *i);
void		expand_variable(t_data *data, char *new_str,
				char *arg, t_state *st);
void		expand_exit_status(t_data *data, char *new_str, t_state *st);
int			env_len(char *str, int i);
int			nbr_len(int n);
char		*get_env_value(t_data *data, char *var_name);
int			find_env_var(char **envp, char *var_name);
void		process_heredocs(t_command *command, t_data *data);

/* executor.c */
void		executor(t_data *data, t_command *command, t_built_in *builtins);
void		execute_child(t_data *data, t_command *command,
				t_built_in *builtins);
int			handle_redirections(t_command *command);
int			is_parent_builtin(t_command *command);
void		execute_parent_builtin(t_data *data, t_command *command,
				t_built_in *builtins);

/* init & free */
t_data		*init_data(char **envp, int i, int count);
t_command	*init_command(void);
void		declare(t_data *data, t_command *command, char *line);
void		analyse_line(t_data *data, t_built_in *builtins,
				t_command *command, char *line);
void		free_data_command(t_data *data, t_command *command);
void		free_command(t_command *command);
void		free_redirs(t_redir *redirs);
void		free_split(char **arr);
void		setup_signals(void);

#endif	
