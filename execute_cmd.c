/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:47:35 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 18:48:02 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*search_in_paths(char **paths, char *cmd)
{
	char	*full_path;
	char	*tmp;
	int		i;

	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (NULL);
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
	}
	return (NULL);
}

static char	*get_cmd_path(t_data *data, char *cmd)
{
	char	**paths;
	char	*path_var;
	char	*found_path;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_var = get_env_value(data, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	found_path = search_in_paths(paths, cmd);
	free_split(paths);
	return (found_path);
}

static void	try_exec_builtin(t_data *data, t_command *cmd, t_built_in *builtins)
{
	int	i;

	i = 0;
	while (builtins[i].cmds)
	{
		if (ft_strcmp(builtins[i].cmds, cmd->args[0]) == 0)
			exit(builtins[i].func(data, cmd));
		i++;
	}
}

static void	exec_external_cmd(t_data *data, t_command *command)
{
	char	*path;
	int		exec_errno;

	path = get_cmd_path(data, command->args[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	execve(path, command->args, data->envp);
	exec_errno = errno;
	ft_putstr_fd("minishell: ", 2);
	perror(command->args[0]);
	free(path);
	if (exec_errno == ENOENT)
		exit(127);
	exit(126);
}

void	execute_child(t_data *data, t_command *command, t_built_in *builtins)
{
	if (handle_redirections(command) != 0)
		exit(1);
	if (command->args == NULL || command->args[0] == NULL)
		exit(0);
	try_exec_builtin(data, command, builtins);
	exec_external_cmd(data, command);
}
