/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:59:29 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 18:08:47 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_data *data, char *var_name)
{
	int		index;
	int		name_len;
	char	*var_string;

	index = find_env_var(data->envp, var_name);
	if (index == -1)
		return (NULL);
	var_string = data->envp[index];
	name_len = ft_strlen(var_name);
	if (var_string[name_len] == '=')
		return (&var_string[name_len + 1]);
	return (NULL);
}

int	nbr_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

int	env_len(char *str, int i)
{
	int	l;

	l = 0;
	if (str[i] == '?')
		return (1);
	while (str[i + l] && (ft_isalnum(str[i + l]) || str[i + l] == '_'))
		l++;
	return (l);
}

void	expand_exit_status(t_data *data, char *new_str, t_state *st)
{
	char	*exit_str;

	exit_str = ft_itoa(data->last_exit_status);
	ft_strlcpy(&new_str[st->j], exit_str, ft_strlen(exit_str) + 1);
	st->j += ft_strlen(exit_str);
	free(exit_str);
	st->i += 2;
}

void	expand_variable(t_data *data, char *new_str, char *arg, t_state *st)
{
	char	*name;
	char	*value;
	int		name_len;

	name_len = env_len(arg, st->i + 1);
	if (name_len == 0)
	{
		new_str[st->j++] = arg[st->i++];
		return ;
	}
	name = ft_substr(arg, st->i + 1, name_len);
	value = get_env_value(data, name);
	free(name);
	if (value)
	{
		ft_strlcpy(&new_str[st->j], value, ft_strlen(value) + 1);
		st->j += ft_strlen(value);
	}
	st->i += (name_len + 1);
}
