/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 23:04:44 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 17:23:23 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

struct	s_data;
struct	s_built_in;
struct	s_command;

typedef	struct s_data
{
	int					last_exit_status;
	char				**args;
	char				**envp;
}	t_data;

typedef struct s_parse_state
{
	int	i;
	int	j;
}	t_parse_state;

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

typedef struct s_command
{
	char				**args;
	t_redir				*redirs;
	struct s_command	*next;
}	t_command;

typedef	int (*t_builtin_func)(t_data *data, t_command *command);

typedef	struct s_built_in
{
	char			*cmds;
	t_builtin_func	func;
}	t_built_in;

void		add_env_var(t_data *data, char *new_var_str);
void		set_env_variable(t_data *data, char *var_name, char *value);
void		executor(t_data *data, t_command *command, t_built_in *builtins);
void		get_args(t_data *data, char *line);
void		manage_env(t_data *data);
void		analyse_line(t_data *data, t_built_in *builtins, t_command *command, char *line);
void		declare(t_data *data, t_command *command, char *line);
void		free_split(char **arr);
void		free_command(t_command *command);
void		free_data_command(t_data *data, t_command *command);
void		update_pwd_vars(t_data *data, char *old_pwd);
void		sort_env_array(char **envp_copy);
void		print_one_env_for_export(char *env_str);
void		ft_putstr_fd(char *s, int fd);
void		print_env(t_data *data);
void		handle_export_arg(t_data *data, t_command *command ,char *arg);
void		expand_and_copy(t_data *data, char *n_s, int *j, char *arg, int *i);
void		remove_var_from_env(t_data *data, int index_to_remove, int count);
int			find_env_var(char **envp, char *var_name);
int			calculate_final_len(t_data *data, char *arg);
int			cd_error(char *path, char *old_pwd);
int			get_expanded_len(t_data *data, char *str, int *i);	
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_strcmp(char *s1, char *s2);
int			echo(t_data *data, t_command *command);
int			cd(t_data *data, t_command *command);
int			pwd(t_data *data, t_command *command);
int			export(t_data *data, t_command *command);
int			unset(t_data *data, t_command *command);
int			env(t_data *data, t_command *command);
int			is_n_flag(const char *arg);
int			env_len(char *str, int i);
int			b_exit(t_data *data, t_command *command);
int			escape_spaces(char *str, int i);
int			escape_char(char *line, int i, char c);
int			nb_of_args(char *line);
int			ft_strcmp(char *s1, char *s2);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(char c);
int			is_valid_identifier(char *name);
int			get_arg_end(char *line, int i);
int			is_numeric(char *str);
char		*ft_itoa(int n);
char		**ft_split(char const *s, char c);
char		**duplicate_env(t_data *data);
char		*ft_strdup(const char *s1);
char		*ft_strchr(const char *s, int c);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*get_cd_path(t_data *data, t_command *command);
char		*get_value(char *str, int i);
char		*get_name(char *str, int *i);
char		*get_env_value(t_data *data, char *var_name);
char		*build_clean_arg(t_data *data, char *arg, char str);
char		*ft_strtrim(char const *s1, char const *set);
long long	ft_atoi(const char *str);
size_t		ft_strlcpy(char *dest, const char *src, size_t n);
size_t		ft_strlen(const char *str);
t_command	*init_command(void);
t_data		*init_data(char **envp, int i, int count);

#endif	