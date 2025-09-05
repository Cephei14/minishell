/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_env_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:46:51 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 14:04:12 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset(t_data *data, t_command *command)
{
	int		i;
	int		index_to_remove;
	char	*trimmed_arg;

	i = 1;
	while (command->args[i])
	{
		trimmed_arg = ft_strtrim(command->args[i], " \t\n\v\f\r");
		if (!is_valid_identifier(trimmed_arg))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(trimmed_arg, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
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

int env(t_data *data, t_command *command)
{
	int	i;

	(void)command;
	i = 0;
	while (data->envp[i])
	{
		if (ft_strchr(data->envp[i], '='))
			printf("%s\n", data->envp[i]);
		i++;
	}
    return (0);
}

int	b_exit(t_data *data, t_command *command)
{
	long long	exit_status;

	if (command->args[1] == NULL)
	{
		printf("exit\n");
		exit(data->last_exit_status);
	}
	if (!is_numeric(command->args[1]))
	{
		printf("exit\n");
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(command->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	if (data->args[2] != NULL)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	printf("exit\n");
	exit_status = ft_atoi(command->args[1]);
	exit(exit_status % 256);
	return (0);
}
