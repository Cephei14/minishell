/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_pwd_cd_export.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:46:51 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 17:27:26 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int echo(t_data *data, t_command *command)
{
    int i;
    int newline;

	(void)data;
    i = 1;
    newline = 1;
	while (command->args[i] && is_n_flag(command->args[i]))
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

	if (command->args[2] != NULL)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		perror("minishell: cd");
		return (1);
	}
	path = get_cd_path(data, command);
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

static int	process_export_arg(t_data *data, t_command * command,char *arg)
{
	char	*name;
	char	*eq_pos;

	eq_pos = ft_strchr(arg, '=');
	if (eq_pos)
		name = ft_substr(arg, 0, eq_pos - arg);
	else
		name = ft_strdup(arg);
	if (!is_valid_identifier(name))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		free(name);
		return (1);
	}
	handle_export_arg(data, command, arg); // Assuming handle_export_arg only needs arg
	free(name);
	return (0);
}
int	export(t_data *data, t_command *command)
{
	int	i;
	int	ret_status;

	ret_status = 0;
	if (command->args[1] == NULL)
	{
		print_env(data);
		return (0);
	}
	i = 1;
	while (command->args[i])
	{
		if (process_export_arg(data, command, command->args[i]) != 0)
			ret_status = 1;
		i++;
	}
	return (ret_status);
}
