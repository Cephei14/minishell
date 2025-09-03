/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_with_$.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:34:51 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/03 21:38:49 by rdhaibi          ###   ########.fr       */
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
			builtins[i].func(data);
			return;
		}
		i++;
	}
	printf("minishell: %s: command not found\n", data->args[0]);
}

void get_args(t_data *data, char *line, int l, int i)
{
	int j;
	int start;

	data->args = malloc(sizeof(char *) * (l + 1));
	if(!data->args)
		return;
	j = 0;
	while(j < l)
	{
		i = escape_spaces(line, i);
		start = i;
		if(line[i] == 39)
			i = escape_char(line, i, 39);
		else if(line[i] == 34)
			i = escape_char(line, i, 34);
		else
		{
			while(line[i] && line[i] != ' ' && line[i] != 39 && line[i] != 34)
				i++;
		}
		data->args[j] = ft_substr(line, start, i - start);
		j++;
	}
	data->args[l] = NULL;
	manage_env(data); //This function is to change normal args like ($HOME) to it's real value (/home/user), in the file "args_after_$.c" you can find next function..
}

int nb_of_args(char *line)
{
	int i;
	int k;

	i = 0;
	k = 0;
	while(line[i])
	{
		i = escape_spaces(line, i);
		if (!line[i])
			break;
		k++;
		if (line[i] == 39)
			i = escape_char(line, i, 39);
		else if(line[i] == 34)
			i = escape_char(line, i, 34);
		else
		{
			while(line[i] && line[i] != ' ' && line[i] != 39 && line[i] != 34)
				i++;
		}
	}
	return (k);
}

void analyse_line(t_data *data, t_built_in *builtins, char *line)
{
	int l;

	l = nb_of_args(line); //We should know how many args first in the line so we can allocate memory for them.( in this case : || echo "abc" "def" $HOME || there are 4)
	get_args(data, line, l, 0); //Here i already put the args in the data structure (data->args) but without changing $HOME to it's real value.
	compare_cmd(data, builtins);
}
