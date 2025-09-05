/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:02:29 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 19:04:54 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_process_logic(t_data *d, t_command *cmd,
		t_built_in *b, t_pipe_fds *fds)
{
	if (fds->in_fd != STDIN_FILENO)
	{
		dup2(fds->in_fd, STDIN_FILENO);
		close(fds->in_fd);
	}
	if (cmd->next)
	{
		close(fds->pipe_fd[0]);
		dup2(fds->pipe_fd[1], STDOUT_FILENO);
		close(fds->pipe_fd[1]);
	}
	execute_child(d, cmd, b);
}

static void	parent_process_logic(t_command *cmd, t_pipe_fds *fds)
{
	if (fds->in_fd != STDIN_FILENO)
		close(fds->in_fd);
	if (cmd->next)
	{
		close(fds->pipe_fd[1]);
		fds->in_fd = fds->pipe_fd[0];
	}
}

static void	wait_for_children(t_data *data, pid_t last_pid)
{
	int		status;
	pid_t	waited_pid;

	while (1)
	{
		waited_pid = wait(&status);
		if (waited_pid <= 0)
			break ;
		if (waited_pid == last_pid)
		{
			if (WIFEXITED(status))
				data->last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				data->last_exit_status = 128 + WTERMSIG(status);
		}
	}
}

static pid_t	launch_pipeline(t_data *d, t_command *cmd, t_built_in *b)
{
	t_pipe_fds	fds;
	pid_t		last_pid;
	t_command	*tmp;

	fds.in_fd = STDIN_FILENO;
	tmp = cmd;
	last_pid = -1;
	while (tmp)
	{
		if (tmp->next && pipe(fds.pipe_fd) == -1)
			perror("pipe");
		last_pid = fork();
		if (last_pid == -1)
			perror("fork");
		else if (last_pid == 0)
			child_process_logic(d, tmp, b, &fds);
		parent_process_logic(tmp, &fds);
		tmp = tmp->next;
	}
	return (last_pid);
}

void	executor(t_data *data, t_command *command, t_built_in *builtins)
{
	pid_t	last_pid;

	if (!command || !command->args || !command->args[0])
		return ;
	if (command->next == NULL && is_parent_builtin(command))
	{
		execute_parent_builtin(data, command, builtins);
		return ;
	}
	last_pid = launch_pipeline(data, command, builtins);
	wait_for_children(data, last_pid);
}
