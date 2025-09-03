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

void	set_env_variable(t_data *data, char *var_name, char *value);
void	compare_cmd(t_data *data, t_built_in *builtins);
void	get_args(t_data *data, char *line, int l, int i);
void	manage_env(t_data *data);
void	analyse_line(t_data *data, t_built_in *builtins, char *line);
void	declare(t_data *data, char *line);
void	analyse_line(t_data *data, t_built_in *builtins, char *str);
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
int		ft_isalnum(char c);
char	**ft_split(char const *s, char c);
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);
size_t	ft_strlcpy(char *dest, const char *src, size_t n);
size_t	ft_strlen(const char *str);

#endif