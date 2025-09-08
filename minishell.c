/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 23:04:35 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/08 18:41:36 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_line_processing(t_data *data,
			t_command **command, char *line)
{
	if (*line)
		add_history(line);
	if (data->args)
		free_split(data->args);
	if (*command)
		free_command(*command);
	*command = init_command();
	if (!*command)
	{
		free(line);
		return (0);
	}
	declare(data, *command, line);
	free(line);
	return (1);
}

static int	process_line(t_data *data, t_command **command)
{
	char	*line;

	g_signal_received = 0;
	line = readline("minishell> ");
	if (g_signal_received == SIGINT)
		data->last_exit_status = 130;
	if (line == NULL)
	{
		printf("exit\n");
		return (0);
	}
	return (handle_line_processing(data, command, line));
}

int	main(int ac, char **av, char **envp)
{
	t_data		*data;
	t_command	*command;
	int			exit_status;

	(void)ac;
	(void)av;
	setup_signals();
	data = init_data(envp, 0, 0);
	if (!data)
		return (1);
	command = NULL;
	while (process_line(data, &command))
		;
	exit_status = data->last_exit_status;
	free_data_command(data, command);
	return (exit_status);
}
