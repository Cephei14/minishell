/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:00:00 by gemini            #+#    #+#             */
/*   Updated: 2025/09/05 16:07:01 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cmd_path(t_data *data, char *cmd);
void	execute_child(t_data *data, t_command *command, t_built_in *builtins);
int	handle_redirections(t_command *command);
int	is_parent_builtin(t_command *command);
void	execute_parent_builtin(t_data *data, t_command *command, t_built_in *builtins);

int	handle_redirections(t_command *command)
{
	int	fd;
	t_redir	*redir;

	redir = command->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		{
			// Note: HEREDOC needs special handling before this point.
			fd = open(redir->filename, O_RDONLY);
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				perror(redir->filename);
				return (1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			if (redir->type == REDIR_APPEND)
				fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				perror(redir->filename);
				return (1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		redir = redir->next;
	}
	return (0);
}

char	*get_cmd_path(t_data *data, char *cmd)
{
	char	**paths;
	char	*path_var;
	char	*full_path;
	char	*tmp;
	int		i;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_var = get_env_value(data, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
	}
	free_split(paths);
	return (NULL);
}

void	execute_child(t_data *data, t_command *command, t_built_in *builtins)
{
	char	*path;
	int		i;

	if (handle_redirections(command) != 0)
		exit(1);
	if (command->args == NULL || command->args[0] == NULL)
		exit(0);
	i = 0;
	while (builtins[i].cmds)
	{
		if (ft_strcmp(builtins[i].cmds, command->args[0]) == 0)
			exit(builtins[i].func(data, command));
		i++;
	}
	path = get_cmd_path(data, command->args[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	execve(path, command->args, data->envp);
	int exec_errno = errno;
	ft_putstr_fd("minishell: ", 2);
	perror(command->args[0]);
	free(path);
	if (exec_errno == ENOENT)
		exit(127);
	exit(126);
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

void	execute_parent_builtin(t_data *data, t_command *command, t_built_in *builtins)
{
	int	i;
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (handle_redirections(command) != 0)
	{
		data->last_exit_status = 1;
	}
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

void	executor(t_data *data, t_command *command, t_built_in *builtins)
{
	int			pipe_fd[2];
	int			in_fd;
	pid_t		last_pid;
	t_command	*tmp;
	int			status;
	pid_t		waited_pid;

	last_pid = -1;
	if (!command || !command->args || !command->args[0])
		return ;
	if (command->next == NULL && is_parent_builtin(command))
		return (execute_parent_builtin(data, command, builtins));
	in_fd = STDIN_FILENO;
	tmp = command;
	while (tmp)
	{
		if (tmp->next && pipe(pipe_fd) == -1)
			perror("pipe");
		last_pid = fork();
		if (last_pid == -1)
			perror("fork");
		else if (last_pid == 0)
		{
			if (in_fd != STDIN_FILENO && (dup2(in_fd, STDIN_FILENO) != -1))
				close(in_fd);
			if (tmp->next && (close(pipe_fd[0]), dup2(pipe_fd[1], STDOUT_FILENO) != -1))
				close(pipe_fd[1]);
			execute_child(data, tmp, builtins);
		}
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (tmp->next)
		{
			close(pipe_fd[1]);
			in_fd = pipe_fd[0];
		}
		tmp = tmp->next;
	}
	while ((waited_pid = wait(&status)) > 0)
	{
		if (waited_pid == last_pid)
		{
			if (WIFEXITED(status))
				data->last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				data->last_exit_status = 128 + WTERMSIG(status);
		}
	}
}
