/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_before.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:32:42 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/04 22:34:01 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void compare_cmd(t_data *data, t_built_in *builtins)
{
	int i;

	i = 0;
	if (data->args == NULL || data->args[0] == NULL)
    	return;
	while(builtins[i].cmds)
	{
		if (ft_strcmp(builtins[i].cmds, data->args[0]) == 0)
		{
			data->last_exit_status = builtins[i].func(data);
			return;
		}
		i++;
	}
	printf("minishell: %s: command not found\n", data->args[0]);
	data->last_exit_status = 127;
}

int	get_arg_end(char *line, int i)
{
	char quote_char;

	quote_char = 0;
	while (line[i] && line[i] != ' ' && line[i] != '\t')
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			quote_char = line[i];
			i++;
			while (line[i] && line[i] != quote_char)
				i++; // This loop can go past the end if no closing quote
            if (line[i] == quote_char)
                i++;
		}
		else
			i++;
	}
	return (i);
}

int nb_of_args(char *line)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while(line[i])
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (line[i])
		{
			count++;
			i = get_arg_end(line, i);
		}
	}
	return (count);
}

void get_args(t_data *data, char *line)
{
	int i;
	int j;
	int start;
	int arg_count;

	i = 0;
	j = 0;
	arg_count = nb_of_args(line);
	data->args = malloc(sizeof(char *) * (arg_count + 1));
	if(!data->args)
		return;
	while(j < arg_count)
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		start = i;
		i = get_arg_end(line, start);
		data->args[j] = ft_substr(line, start, i - start);
		j++;
	}
	data->args[j] = NULL;
}

void analyse_line(t_data *data, t_built_in *builtins, char *line)
{
	get_args(data, line);
	if (data->args && data->args[0])
	{
		manage_env(data);
		compare_cmd(data, builtins);
	}
}