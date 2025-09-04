/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_after_$.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:34:51 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/03 21:06:11 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void remove_quotes(t_data *data, int i)
{
	char *str;
	int x;
	int j;
	int l;

	x = 0;
	j = 1;
	l = ft_strlen(data->args[i]) - 2;
	str = malloc(sizeof(char) * (l + 1));
	while(x < l)
	{
		str[x] = data->args[i][j];
		x++;
		j++;
	}
	str[x] = '\0';
	free(data->args[i]);
	data->args[i] = str;
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

int	get_total_len(char *arg_str, int i, int total_len)
{
	int		l;
	char	*env_name;
	char	*env_value;

	while (arg_str[i])
	{
		if (arg_str[i] == '$')
		{
			i++;
			l = env_len(arg_str, i);
			env_name = ft_substr(arg_str, i, l);
			env_value = getenv(env_name); //$? maybe here.
			if (env_value)
				total_len += ft_strlen(env_value);
			free(env_name);
			i += l;
		}
		else
		{
			total_len++;
			i++;
		}
	}
	return (total_len);
}

char	*expand_argument(char *str, int len, int i, int x)
{
	char	*new_arg;
	int		l;
	char	*env_name;

	new_arg = malloc(sizeof(char) * (len + 1));
	if (!new_arg)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			l = env_len(str, i);
			env_name = ft_substr(str, i, l);
			copy_env_value(new_arg, &x, env_name);
			free(env_name);
			i += l;
		}
		else
			new_arg[x++] = str[i++];
	}
	new_arg[x] = '\0';
	return (new_arg);
}

void	manage_env(t_data *data)
{
	int		i;
	char	*new_arg;
	int		total_len;

	i = 0;
	while (data->args[i])
	{
		if (data->args[i][0] == 39)
			remove_quotes(data, i);
		else
		{
			if (data->args[i][0] == 34)
				remove_quotes(data, i);
			total_len = get_total_len(data->args[i], 0, 0);
			new_arg = expand_argument(data->args[i], total_len, 0, 0);
			free(data->args[i]);
			data->args[i] = new_arg;
		}
		i++;
	}
}
