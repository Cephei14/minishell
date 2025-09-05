/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_plus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:19:33 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 17:22:36 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_var_from_env(t_data *data, int index_to_remove, int count)
{
	char	**new_envp;
	int		i;
	int		j;

	while (data->envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * count);
	if (!new_envp)
		return;
	i = 0;
	j = 0;
	while (data->envp[i])
	{
		if (i != index_to_remove)
		{
			new_envp[j] = data->envp[i];
			j++;
		}
		i++;
	}
	new_envp[j] = NULL;
	free(data->envp[index_to_remove]);
	free(data->envp);
	data->envp = new_envp;
}
