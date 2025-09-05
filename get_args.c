/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:56:28 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 17:56:47 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_empty_args(t_data *data, int i, int j)
{
	int		count;
	char	**new_args;

	count = 0;
	while (data->args && data->args[i])
	{
		if (data->args[i][0] != '\0')
			count++;
		i++;
	}
	new_args = malloc(sizeof(char *) * (count + 1));
	if (!new_args)
		return ;
	i = 0;
	while (data->args && data->args[i])
	{
		if (data->args[i][0] != '\0')
			new_args[j++] = data->args[i];
		else
			free(data->args[i]);
		i++;
	}
	new_args[j] = NULL;
	free(data->args);
	data->args = new_args;
}

static int	is_metachar(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>');
}

static int	find_token_end(char *line, int i)
{
	char	quote_char;

	if (line[i] == '|' || line[i] == '<' || line[i] == '>')
	{
		if ((line[i] == '<' && line[i + 1] == '<')
			|| (line[i] == '>' && line[i + 1] == '>'))
			return (i + 2);
		return (i + 1);
	}
	while (line[i] && !is_metachar(line[i]))
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			quote_char = line[i++];
			while (line[i] && line[i] != quote_char)
				i++;
			if (line[i])
				i++;
		}
		else
			i++;
	}
	return (i);
}

static int	count_tokens(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (line[i])
		{
			count++;
			i = find_token_end(line, i);
		}
	}
	return (count);
}

void	get_args(t_data *data, char *line)
{
	int	i;
	int	j;
	int	start;
	int	arg_count;

	i = 0;
	j = 0;
	arg_count = count_tokens(line);
	data->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!data->args)
		return ;
	while (j < arg_count)
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		start = i;
		i = find_token_end(line, i);
		data->args[j] = ft_substr(line, start, i - start);
		j++;
	}
	data->args[j] = NULL;
}
