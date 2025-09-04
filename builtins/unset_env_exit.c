/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_env_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:46:51 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/04 22:24:45 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset(t_data *data)
{
	int		i;
	int		index_to_remove;
	char	*trimmed_arg;

	i = 1;
	while (data->args[i])
	{
		trimmed_arg = ft_strtrim(data->args[i], " \t\n\v\f\r");
		if (!is_valid_identifier(trimmed_arg))
			printf("minishell: unset: `%s': not a valid identifier\n", \
				trimmed_arg);
		else
		{
			index_to_remove = find_env_var(data->envp, trimmed_arg);
			if (index_to_remove != -1)
				remove_var_from_env(data, index_to_remove);
		}
		free(trimmed_arg);
		i++;
	}
	return (0);
}

int env(t_data *data)
{
	int	i;

	i = 0;
	while (data->envp[i])
	{
		if (ft_strchr(data->envp[i], '='))
			printf("%s\n", data->envp[i]);
		i++;
	}
    return (0);
}

int	b_exit(t_data *data)
{
	long long	exit_status;

	printf("exit\n");
	if (data->args[1] == NULL)
		exit(data->last_exit_status);
	if (is_numeric(data->args[1]))
	{
		if (data->args[2] != NULL)
		{
			printf("minishell: exit: too many arguments\n");
			return (1);
		}
		exit_status = ft_atoi(data->args[1]);
		exit(exit_status % 256);
	}
	else
	{
		printf("minishell: exit: %s: numeric argument required\n", \
			data->args[1]);
		exit(2);
	}
	return (0);
}
