/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print_sorted_env.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:46:51 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/04 16:05:09 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_one_env_for_export(char *env_str)
{
	int	i;

	i = 0;
	printf("declare -x ");
	while (env_str[i] && env_str[i] != '=')
	{
		printf("%c", env_str[i]);
		i++;
	}
	if (env_str[i] == '=')
		printf("=\"%s\"", &env_str[i + 1]);
	printf("\n");
}

char	**duplicate_env(t_data *data)
{
	char	**new_envp;
	int		count;
	int		i;

	count = 0;
	while (data->envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_envp[i] = data->envp[i];
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

void	sort_env_array(char **envp_copy)
{
	char	*temp_str;
	int		count;
	int		i;
	int		j;

	count = 0;
	while (envp_copy[count])
		count++;
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(envp_copy[j], envp_copy[j + 1]) > 0)
			{
				temp_str = envp_copy[j];
				envp_copy[j] = envp_copy[j + 1];
				envp_copy[j + 1] = temp_str;
			}
			j++;
		}
		i++;
	}
}

void	print_env(t_data *data)
{
	char	**envp_copy;
	int		i;

	envp_copy = duplicate_env(data);
	if (!envp_copy)
		return;
	sort_env_array(envp_copy);
	i = 0;
	while (envp_copy[i])
	{
		print_one_env_for_export(envp_copy[i]);
		i++;
	}
	free(envp_copy);
}
