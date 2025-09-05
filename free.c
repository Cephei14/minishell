/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:33:39 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 14:02:35 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_command(t_command *command)
{
	t_command	*current;
	t_command	*next;

	current = command;
	while (current)
	{
		next = current->next;
		if (current->args)
			free_split(current->args);
		free(current);
		current = next;
	}
}

void free_data_command(t_data *data, t_command *command)
{
	if (!data)
		return;
	if (data->args)
		free_split(data->args);
	if (data->envp)
		free_split(data->envp);
	free(data);
	free_command(command);
}

void free_split(char **arr)
{
    int i;

    i = 0;
    if (!arr)
        return;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}