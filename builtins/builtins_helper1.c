/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_helper1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:46:51 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/03 20:53:16 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_env_var(t_data *data, char *new_var_str)
{
	char	**new_envp;
	int		count;
	int		i;

	count = 0;
	while (data->envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return;
	i = 0;
	while (i < count)
	{
		new_envp[i] = data->envp[i];
		i++;
	}
	new_envp[i] = new_var_str;
	new_envp[i + 1] = NULL;
	free(data->envp);
	data->envp = new_envp;
}

int	find_env_var(char **envp, char *var_name)
{
	int		i;
	int		name_len;

	i = 0;
	name_len = ft_strlen(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, name_len) == 0
			&& envp[i][name_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

void	set_env_variable(t_data *data, char *var_name, char *value)
{
	int		index;
	char	*temp_str;
	char	*new_var_str;

	temp_str = ft_strjoin(var_name, "=");
	new_var_str = ft_strjoin(temp_str, value);
	free(temp_str);
	index = find_env_var(data->envp, var_name);
	if (index == -1)
		add_env_var(data, new_var_str);
	else
	{
		free(data->envp[index]);
		data->envp[index] = new_var_str;
	}
}

void	update_pwd_vars(t_data *data, char *old_pwd)
{
	char	*new_pwd;

	set_env_variable(data, "OLDPWD", old_pwd);
	free(old_pwd);
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		set_env_variable(data, "PWD", new_pwd);
		free(new_pwd);
	}
}

char	*get_cd_path(t_data *data)
{
	char	*path;

	if (data->args[1] == NULL)
	{
		path = getenv("HOME");
		if (path == NULL)
			printf("minishell: cd: HOME not set\n");
		return (path);
	}
	return (data->args[1]);
}
