/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:33:39 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/03 20:34:11 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_data(t_data *data)
{
	if (!data)
		return;
	if (data->args)
		free_split(data->args);
	if (data->envp)
		free_split(data->envp);
	free(data);
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