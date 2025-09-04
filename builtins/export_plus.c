/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_plus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 15:48:48 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/04 16:25:14 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(char *name)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(name[i]) || name[i] == '_'))
		return (0);
	i++;
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

// FIX: get_value now correctly handles cases with and without an '='
char	*get_value(char *str, int i)
{
	int	l;

	// If there is an '=' at index i, the value starts at i + 1.
	if (str[i] == '=')
	{
		i++;
		l = ft_strlen(str) - i;
		return (ft_substr(str, i, l));
	}
	// If there is no '=', there is no value. Return NULL.
	return (NULL);
}

// FIX: get_name now correctly validates the name and finds the '='
char	*get_name(char *str, int *i)
{
	char	*name;

	*i = 0;
	while (str[*i] && str[*i] != '=')
		(*i)++;
	name = ft_substr(str, 0, *i);
	if (!is_valid_identifier(name))
	{
		free(name);
		return (NULL);
	}
	return (name);
}

void	handle_export_arg(t_data *data, char *arg)
{
	int		l;
	char	*name;
	char	*value;

	l = 0;
	name = get_name(arg, &l);
	if (name)
	{
		value = get_value(arg, l);
		set_env_variable(data, name, value);
		free(name);
		if (value)
			free(value);
	}
	else
		printf("minishell: export: `%s': not a valid identifier\n", arg);
}