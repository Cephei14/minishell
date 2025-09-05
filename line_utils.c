/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:32:42 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 18:11:30 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char *arg)
{
	if (!arg)
		return (0);
	if (ft_strcmp(arg, "|") == 0 || ft_strcmp(arg, ">") == 0
		|| ft_strcmp(arg, ">>") == 0 || ft_strcmp(arg, "<") == 0
		|| ft_strcmp(arg, "<<") == 0)
		return (1);
	return (0);
}

int	count_cmd_args(char **args, int start)
{
	int	count;
	int	i;

	count = 0;
	i = start;
	while (args[i] && ft_strcmp(args[i], "|") != 0)
	{
		if (is_operator(args[i]))
			i++;
		else
			count++;
		i++;
	}
	return (count);
}

t_redir	*new_redir(char *filename, t_redir_type type)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->filename = ft_strdup(filename);
	redir->type = type;
	redir->next = NULL;
	return (redir);
}

void	analyse_line(t_data *data, t_built_in *builtins,
			t_command *command, char *line)
{
	get_args(data, line);
	manage_env(data);
	remove_empty_args(data, 0, 0);
	fill_commands(data, command, 0);
	executor(data, command, builtins);
}
