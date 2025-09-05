/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:33:39 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 17:53:48 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirs(t_redir *redirs)
{
	t_redir	*current;
	t_redir	*next;

	current = redirs;
	while (current)
	{
		next = current->next;
		free(current->filename);
		free(current);
		current = next;
	}
}

void	free_command(t_command *command)
{
	t_command	*current;
	t_command	*next;

	current = command;
	while (current)
	{
		next = current->next;
		if (current->args)
			free_split(current->args);
		if (current->redirs)
			free_redirs(current->redirs);
		free(current);
		current = next;
	}
}

void	free_data_command(t_data *data, t_command *command)
{
	if (!data)
		return ;
	if (data->args)
		free_split(data->args);
	if (data->envp)
		free_split(data->envp);
	free(data);
	free_command(command);
}

void	free_split(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
