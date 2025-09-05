/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:35:51 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 17:51:11 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	*init_data(char **envp, int i, int count)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->args = NULL;
	data->last_exit_status = 0;
	while (envp[count])
		count++;
	data->envp = malloc(sizeof(char *) * (count + 1));
	if (!data->envp)
	{
		free(data);
		return (NULL);
	}
	while (i < count)
	{
		data->envp[i] = ft_strdup(envp[i]);
		i++;
	}
	data->envp[i] = NULL;
	return (data);
}

t_command	*init_command(void)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->args = NULL;
	command->redirs = NULL;
	command->next = NULL;
	return (command);
}

void	declare(t_data *data, t_command *command, char *line)
{
	t_built_in	builtins[8];

	builtins[0].cmds = "echo";
	builtins[0].func = &echo;
	builtins[1].cmds = "cd";
	builtins[1].func = &cd;
	builtins[2].cmds = "pwd";
	builtins[2].func = &pwd;
	builtins[3].cmds = "export";
	builtins[3].func = &export;
	builtins[4].cmds = "unset";
	builtins[4].func = &unset;
	builtins[5].cmds = "env";
	builtins[5].func = &env;
	builtins[6].cmds = "exit";
	builtins[6].func = &b_exit;
	builtins[7].cmds = NULL;
	builtins[7].func = NULL;
	analyse_line(data, builtins, command, line);
}
