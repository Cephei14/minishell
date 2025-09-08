/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:40:21 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/08 16:00:48 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*generate_heredoc_filename(void)
{
	static int	heredoc_count;
	char		*heredoc_num_str;
	char		*filename;

	heredoc_count = 0;
	heredoc_num_str = ft_itoa(heredoc_count++);
	if (!heredoc_num_str)
		return (NULL);
	filename = ft_strjoin("/tmp/minishell_heredoc_", heredoc_num_str);
	free(heredoc_num_str);
	return (filename);
}

static int	read_heredoc_input(t_redir *redir)
{
	char	*line;
	int		fd;
	char	*temp_filename;

	temp_filename = generate_heredoc_filename();
	if (!temp_filename)
		return (1);
	fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(temp_filename);
		return (1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->filename) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	close(fd);
	free(redir->filename);
	redir->filename = temp_filename;
	return (0);
}

void	process_heredocs(t_command *command, t_data *data)
{
	t_command	*current_cmd;
	t_redir		*current_redir;

	current_cmd = command;
	while (current_cmd)
	{
		current_redir = current_cmd->redirs;
		while (current_redir)
		{
			if (current_redir->type == REDIR_HEREDOC)
			{
				if (read_heredoc_input(current_redir) != 0)
				{
					perror("minishell: heredoc failed");
					data->parse_error = 1;
					return ;
				}
			}
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
}
