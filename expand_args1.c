/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:54:33 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 18:08:13 by rdhaibi          ###   ########.fr       */
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
	if (str[*i] == '?')
	{
		(*i)++;
		return (nbr_len(data->last_exit_status));
	}
	name_len = env_len(str, *i);
	if (name_len == 0)
		return (1);
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

void	handle_expansion(t_data *d, char *n_s, char *arg, t_state *st)
{
	if (arg[st->i + 1] == '?')
		expand_exit_status(d, n_s, st);
	else
		expand_variable(d, n_s, arg, st);
}

char	*build_clean_arg(t_data *data, char *arg, char quote_char)
{
	char			*new_arg;
	t_state			st;

	st.i = 0;
	st.j = 0;
	new_arg = malloc(sizeof(char) * (calculate_final_len(data, arg) + 1));
	if (!new_arg)
		return (NULL);
	while (arg[st.i])
	{
		if ((arg[st.i] == '\'' || arg[st.i] == '\"') && quote_char == 0)
			quote_char = arg[st.i++];
		else if (arg[st.i] == quote_char)
		{
			quote_char = 0;
			st.i++;
		}
		else if (arg[st.i] == '$' && quote_char != '\'')
			handle_expansion(data, new_arg, arg, &st);
		else
			new_arg[st.j++] = arg[st.i++];
	}
	new_arg[st.j] = '\0';
	return (new_arg);
}

void	manage_env(t_data *data)
{
	int		i;
	char	*clean_arg;

	i = 0;
	while (data->args[i])
	{
		clean_arg = build_clean_arg(data, data->args[i], 0);
		free(data->args[i]);
		data->args[i] = clean_arg;
		i++;
	}
}
