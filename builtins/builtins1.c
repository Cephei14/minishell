/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:46:51 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/03 20:48:25 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int echo(t_data *data)
{
    int i;
    int newline;

    i = 1;
    newline = 1;

    if (data->args[1] && ft_strcmp(data->args[1], "-n") == 0)
    {
        newline = 0;
        i = 2;
    }
    while (data->args[i])
    {
        printf("%s", data->args[i]);
        if (data->args[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    return (0);
}

int pwd(t_data *data)
{
	char	*current_dir;

	(void)data;
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
	{
		perror("minishell: pwd");
		return (1);
	}
	printf("%s\n", current_dir);
	free(current_dir);
	return (0);
}


int	cd(t_data *data)
{
	char	*path;
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		perror("minishell: cd");
		return (1);
	}
	path = get_cd_path(data);
	if (!path)
	{
		free(old_pwd);
		return (1);
	}
	if (chdir(path))
	{
		perror("minishell: cd");
		free(old_pwd);
		return (1);
	}
	update_pwd_vars(data, old_pwd);
	return (0);
}


int export(t_data *data)
{
	int i;
	int l;
	char *name;
	char *value;

	name = NULL;
	l = 0;
	if(!data->args[1])
		print_env(data);
	else
	{
		name = get_name(data->args[1], &l);
		if(name)
			value = get_value(data->args[1], l + 1);
		else
			set_env_variable(data, name, value);
	}
    return (0);
}

