/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:35:51 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/03 21:32:18 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data *init_data(char **envp)
{
	t_data *data;
	int i;
	int count;

	data = malloc(sizeof(t_data));
	if(!data)
		return (NULL);
	data->args = NULL;
	count = 0;
	while(envp[count])
		count++;
	data->envp = malloc(sizeof(char *) * (count + 1));
	if (!data->envp)
	{
		free(data);
		return (NULL);
	}
	i = 0;
	while(i < count)
	{
		data->envp[i] = ft_strdup(envp[i]);
		i++;
	}
	data->envp[i] = NULL;
	return (data);
}

void declare(t_data *data, char *line)
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
	analyse_line(data, builtins, line);//Here we get the args like they are ( || echo "abc" "def" $HOME || -> args[0] = echo, args[1] = "abc", args[2] = 'def' args[3] = $HOME)
}
