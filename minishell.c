/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 23:04:35 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 14:02:36 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_env_value(t_data *data, char *new_arg, int *x, char *env_name)
{
	char	*env_value;
	int		j;

	env_value = get_env_value(data, env_name);
	j = 0;
	if (env_value)
	{
		while (env_value[j])
		{
			new_arg[*x] = env_value[j];
			(*x)++;
			j++;
		}
	}
}

int main(int ac, char **av, char **envp)
{
	char		*line;
	t_data		*data;
	t_command	*command;

	(void)ac;
	(void)av;
	data = init_data(envp, 0, 0);
	command = NULL;
	while(1)
	{
		line = readline("minishell> ");
		if (line == NULL)
		{
			printf("exit\n");
			break;
		}
		if (*line)
			add_history(line);
		if (data->args)
			free_split(data->args);
		if (command)
			free_command(command);
		command = init_command(envp, 0, 0);
		declare(data, command, line);
		free(line);
	}
	free_data_command(data, command);
	return (data->last_exit_status);
}
