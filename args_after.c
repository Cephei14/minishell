/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_after.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:34:51 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/04 17:47:00 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_expanded_len(t_data *data, char *str, int *i)
{
	char	*name;
	char	*value;
	int		len;
	int		name_len;

	(*i)++;
	name_len = env_len(str, *i);
	name = ft_substr(str, *i, name_len);
	value = get_env_value(data, name);
	free(name);
	*i += name_len;
	len = 0;
	if (value)
		len = ft_strlen(value);
	return (len);
}

int	calculate_final_len(t_data *data, char *arg)
{
	int		i;
	int		len;
	char	quote_char;

	i = 0;
	len = 0;
	quote_char = 0;
	while (arg[i])
	{
		if ((arg[i] == '\'' || arg[i] == '\"') && quote_char == 0)
			quote_char = arg[i++];
		else if (arg[i] == quote_char)
		{
			quote_char = 0;
			i++;
		}
		else if (arg[i] == '$' && quote_char != '\'')
			len += get_expanded_len(data, arg, &i);
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

void	expand_and_copy(t_data *data, char *n_s, int *j, char *arg, int *i)
{
	char	*name;
	char	*value;
	int		name_len;

	(*i)++;
	name_len = env_len(arg, *i);
	name = ft_substr(arg, *i, name_len);
	value = get_env_value(data, name);
	free(name);
	if (value)
	{
		ft_strlcpy(&n_s[*j], value, ft_strlen(value) + 1);
		*j += ft_strlen(value);
	}
	*i += name_len;
}

char	*build_clean_arg(t_data *data, char *arg, int i, int j)
{
	char	*new_arg;
	char	quote_char;

	new_arg = malloc(sizeof(char) * (calculate_final_len(data, arg) + 1));
	if (!new_arg)
		return (NULL);
	quote_char = 0;
	while (arg[i])
	{
		if ((arg[i] == '\'' || arg[i] == '\"') && quote_char == 0)
			quote_char = arg[i++];
		else if (arg[i] == quote_char)
		{
			quote_char = 0;
			i++;
		}
		else if (arg[i] == '$' && quote_char != '\'')
			expand_and_copy(data, new_arg, &j, arg, &i);
		else
			new_arg[j++] = arg[i++];
	}
	new_arg[j] = '\0';
	return (new_arg);
}

int	env_len(char *str, int i)
{
	int	l;

	l = 0;
	// Maybe i should handle the `$?` here later
	if (str[i] == '?')
		return (1);
	while (str[i + l] && (ft_isalnum(str[i + l]) || str[i + l] == '_'))
		l++;
	return (l);
}

