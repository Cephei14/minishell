/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:01:57 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 18:02:27 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_redir_back(t_command *cmd, t_redir *redir)
{
	t_redir	*current;

	if (!cmd->redirs)
	{
		cmd->redirs = redir;
		return ;
	}
	current = cmd->redirs;
	while (current->next)
		current = current->next;
	current->next = redir;
}

static int	handle_redirection(t_command *cmd, char **args, int i)
{
	if (ft_strcmp(args[i], ">") == 0)
		add_redir_back(cmd, new_redir(args[i + 1], REDIR_OUT));
	else if (ft_strcmp(args[i], ">>") == 0)
		add_redir_back(cmd, new_redir(args[i + 1], REDIR_APPEND));
	else if (ft_strcmp(args[i], "<") == 0)
		add_redir_back(cmd, new_redir(args[i + 1], REDIR_IN));
	else if (ft_strcmp(args[i], "<<") == 0)
		add_redir_back(cmd, new_redir(args[i + 1], REDIR_HEREDOC));
	return (i + 2);
}

int	parse_one_command(t_data *data, t_command *cmd, int i)
{
	int	arg_idx;

	arg_idx = 0;
	while (data->args[i] && ft_strcmp(data->args[i], "|") != 0)
	{
		if (is_operator(data->args[i]))
		{
			if (!data->args[i + 1] || is_operator(data->args[i + 1]))
			{
				ft_putstr_fd("minishell: syntax error\n", 2);
				data->last_exit_status = 2;
				return (i + 1);
			}
			i = handle_redirection(cmd, data->args, i);
		}
		else
		{
			cmd->args[arg_idx++] = ft_strdup(data->args[i++]);
		}
	}
	cmd->args[arg_idx] = NULL;
	return (i);
}

void	fill_commands(t_data *data, t_command *command, int i)
{
	t_command	*current_cmd;
	int			arg_count;

	current_cmd = command;
	while (data->args && data->args[i])
	{
		arg_count = count_cmd_args(data->args, i);
		current_cmd->args = malloc(sizeof(char *) * (arg_count + 1));
		if (!current_cmd->args)
			return ;
		i = parse_one_command(data, current_cmd, i);
		if (data->args[i] && ft_strcmp(data->args[i], "|") == 0)
		{
			i++;
			current_cmd->next = init_command();
			current_cmd = current_cmd->next;
		}
	}
}

int	find_env_var(char **envp, char *var_name)
{
	int		i;
	int		name_len;

	i = 0;
	name_len = ft_strlen(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, name_len) == 0
			&& (envp[i][name_len] == '=' || envp[i][name_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}
