/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:35:51 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 13:37:23 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data *init_data(char **envp, int i, int count)
{
	t_data *data;

	data = malloc(sizeof(t_data));
	if(!data)
		return (NULL);
	data->args = NULL;
	data->last_exit_status = 0;
	while(envp[count])
		count++;
	data->envp = malloc(sizeof(char *) * (count + 1));
	if (!data->envp)
	{
		free(data);
		return (NULL);
	}
	while(i < count)
	{
		data->envp[i] = ft_strdup(envp[i]);
		i++;
	}
	data->envp[i] = NULL;
	return (data);
}

t_command *init_command(char **envp, int i, int count)
{
	t_command	*command;

	(void)envp;
	(void)i;
	(void)count;
	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->args = NULL;
	command->input_file = NULL;
	command->output_file = NULL;
	command->is_append = 0;
	command->next = NULL;
	return (command);
}

void declare(t_data *data, t_command *command, char *line)
{
	t_built_in builtins[] = {
		{"echo", &echo},
    	{"cd", &cd},
    	{"pwd", &pwd},
    	{"export", &export},
    	{"unset", &unset},
    	{"env", &env},
    	{"exit", &b_exit},
    	{NULL, NULL}
	};
	analyse_line(data, builtins, command, line);
}
