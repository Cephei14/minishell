/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:00:52 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 18:27:28 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_env_var(t_data *data, char *new_var_str)
{
	char	**new_envp;
	int		count;
	int		i;

	count = 0;
	while (data->envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return ;
	i = 0;
	while (i < count)
	{
		new_envp[i] = data->envp[i];
		i++;
	}
	new_envp[i] = new_var_str;
	new_envp[i + 1] = NULL;
	free(data->envp);
	data->envp = new_envp;
}

void	set_env_variable(t_data *data, char *var_name, char *value)
{
	int		index;
	char	*new_var_str;
	char	*temp_str;

	if (value == NULL)
	{
		if (find_env_var(data->envp, var_name) != -1)
			return ;
		new_var_str = ft_strdup(var_name);
	}
	else
	{
		temp_str = ft_strjoin(var_name, "=");
		new_var_str = ft_strjoin(temp_str, value);
		free(temp_str);
	}
	index = find_env_var(data->envp, var_name);
	if (index == -1)
		add_env_var(data, new_var_str);
	else
	{
		free(data->envp[index]);
		data->envp[index] = new_var_str;
	}
}
