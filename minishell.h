#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

struct	s_data;
struct	s_built_in;

typedef	struct s_data //In this structure we store data.
{
	char **args; //Here we store tokens for example : (echo "abc" "def" -> args[0] = echo, args[1] = "abc", args[2] = 'def').
	char **envp; //since we cannot directly modify the original environment variables -> we should make a copy of them in our shell and then do whatever we want to them.
	
}	t_data;

typedef	int (*t_builtin_func)(t_data *data);

typedef	struct s_built_in //In this structure we store each command and it's function.
{
	char			*cmds; //Here are the builtins commands: (echo cd pwd export unset env exit).
	t_builtin_func	func; //Here a function pointer pointing to the function for each command.
	
}	t_built_in;


void	add_env_var(t_data *data, char *new_var_str);
void	set_env_variable(t_data *data, char *var_name, char *value);
void	compare_cmd(t_data *data, t_built_in *builtins);
void	get_args(t_data *data, char *line, int l, int i);
void	manage_env(t_data *data);
void	analyse_line(t_data *data, t_built_in *builtins, char *line);
void	declare(t_data *data, char *line);
void	analyse_line(t_data *data, t_built_in *builtins, char *str);
void	free_split(char **arr);
void	free_data(t_data *data);
void	copy_env_value(t_data *data, char *new_arg, int *x, char *env_name);
void	update_pwd_vars(t_data *data, char *old_pwd);
void	sort_env_array(char **envp_copy);
void	print_one_env_for_export(char *env_str);
void	print_env(t_data *data);
void	handle_export_arg(t_data *data, char *arg);
int		find_env_var(char **envp, char *var_name);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp(char *s1, char *s2);
int		echo(t_data *data);
int		cd(t_data *data);
int		pwd(t_data *data);
int		export(t_data *data);
int		unset(t_data *data);
int		env(t_data *data);
int		b_exit(t_data *data);
int		escape_spaces(char *str, int i);
int		escape_char(char *line, int i, char c);
int		nb_of_args(char *line);
int		ft_strcmp(char *s1, char *s2);
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(char c);
int		is_valid_identifier(char *name);
char	**ft_split(char const *s, char c);
char	**duplicate_env(t_data *data);
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*get_cd_path(t_data *data);
char	*get_value(char *str, int i);
char	*get_name(char *str, int *i);
char	*get_env_value(t_data *data, char *var_name);
size_t	ft_strlcpy(char *dest, const char *src, size_t n);
size_t	ft_strlen(const char *str);
t_data	*init_data(char **envp);

#endif