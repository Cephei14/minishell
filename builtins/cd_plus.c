/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_plus.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 15:35:09 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/04 20:25:17 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cd_path(t_data *data)
{
	char	*path;

	if (data->args[1] == NULL)
	{
		path = get_env_value(data, "HOME");
		if (path == NULL)
			printf("minishell: cd: HOME not set\n");
		return (path);
	}
	else if (ft_strcmp(data->args[1], "-") == 0)
	{
		path = get_env_value(data, "OLDPWD");
		if (path == NULL)
			printf("minishell: cd: OLDPWD not set\n");
		else
			printf("%s\n", path);
		return (path);
	}
	return (data->args[1]);
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


