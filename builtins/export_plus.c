/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_plus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 15:48:48 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/04 18:35:36 by rdhaibi          ###   ########.fr       */
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

char	*get_value(char *str, int i)
{
	int	l;
	int flag;

	flag = 0;
	if (str[i] == '=')
	{
		i++;
		if(str[i] == 34)
		{
			flag = 2;
			i++;
		}
		l = ft_strlen(str) - i - flag;
		return (ft_substr(str, i, l));
	}
	return (NULL);
}

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
	char	*trimmed_arg;

	trimmed_arg = ft_strtrim(arg, " \t\n\v\f\r");
	l = 0;
	name = get_name(trimmed_arg, &l);
	if (name)
	{
		value = get_value(trimmed_arg, l);
		set_env_variable(data, name, value);
		free(name);
		if (value)
			free(value);
	}
	else
		printf("minishell: export: `%s': not a valid identifier\n", trimmed_arg);
	free(trimmed_arg);
}
