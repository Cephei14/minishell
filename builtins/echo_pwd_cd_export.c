/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_pwd_cd_export.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:46:51 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 14:03:13 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int echo(t_data *data, t_command *command)
{
    int i;
    int newline;

    i = 1;
    newline = 1;
	while (command->args[i] && is_n_flag(data->args[i]))
	{
	    newline = 0;
	    i++;
	}
    while (command->args[i])
    {
        printf("%s", command->args[i]);
        if (command->args[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    return (0);
}

int pwd(t_data *data,  t_command *command)
{
	char	*current_dir;

	(void)data;
	(void)command;
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

int	cd(t_data *data,  t_command *command)
{
	char	*path;
	char	*old_pwd;

	(void)command;
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
		return (cd_error(path, old_pwd));
	update_pwd_vars(data, old_pwd);
	return (0);
}

int	export(t_data *data, t_command *command)
{
	int	i;

	if (command->args[1] == NULL)
	{
		print_env(data);
		return (0);
	}
	i = 1;
	while (command->args[i])
	{
		handle_export_arg(data, command, command->args[i]);
		i++;
	}
	return (0);
}
