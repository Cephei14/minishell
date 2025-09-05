/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_plus.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 15:35:09 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 18:20:33 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_n_flag(const char *arg)
{
	int	i;

	if (arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i] == 'n')
		i++;
	if (arg[i] == '\0')
		return (1);
	return (0);
}

char	*get_cd_path(t_data *data, t_command *command)
{
	char	*path;

	if (command->args[1] == NULL)
	{
		path = get_env_value(data, "HOME");
		if (path == NULL)
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (path);
	}
	else if (ft_strcmp(command->args[1], "-") == 0)
	{
		path = get_env_value(data, "OLDPWD");
		if (path == NULL)
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		else
			printf("%s\n", path);
		return (path);
	}
	return (command->args[1]);
}

int	cd_error(char *path, char *old_pwd)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	free(old_pwd);
	return (1);
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
