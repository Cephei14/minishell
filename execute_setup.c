/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_setup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:51:01 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 18:57:38 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	apply_input_redir(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir->filename);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	apply_output_redir(t_redir *redir)
{
	int	fd;
	int	flags;

	if (redir->type == REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(redir->filename, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir->filename);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_redirections(t_command *command)
{
	t_redir	*redir;

	redir = command->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		{
			if (apply_input_redir(redir) != 0)
				return (1);
		}
		else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			if (apply_output_redir(redir) != 0)
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}

int	is_parent_builtin(t_command *command)
{
	char	*cmd_name;

	if (!command->args || !command->args[0])
		return (0);
	cmd_name = command->args[0];
	if (ft_strcmp(cmd_name, "cd") == 0 || ft_strcmp(cmd_name, "unset") == 0
		|| ft_strcmp(cmd_name, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "export") == 0 && command->args[1] != NULL)
		return (1);
	return (0);
}

void	execute_parent_builtin(t_data *data, t_command *command,
			t_built_in *builtins)
{
	int	i;
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (handle_redirections(command) != 0)
		data->last_exit_status = 1;
	else
	{
		i = 0;
		while (builtins[i].cmds)
		{
			if (ft_strcmp(builtins[i].cmds, command->args[0]) == 0)
			{
				data->last_exit_status = builtins[i].func(data, command);
				break ;
			}
			i++;
		}
	}
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
