/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_plus.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 15:35:09 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/04 16:01:54 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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


