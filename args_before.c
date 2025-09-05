/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_before.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:32:42 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 13:57:44 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void compare_cmd(t_data *data, t_command *command, t_built_in *builtins)
{
	int i;

	i = 0;
	if (command->args == NULL || command->args[0] == NULL)
    	return;
	while(builtins[i].cmds)
	{
		if (ft_strcmp(builtins[i].cmds, command->args[0]) == 0)
		{
			data->last_exit_status = builtins[i].func(data, command);
			return;
		}
		i++;
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command->args[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	data->last_exit_status = 127;
}

/********************************************fill_command - start **************************************************/
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

int	parse_one_command(t_data *data, t_command *cmd, int i)
{
	int	arg_idx;

	arg_idx = 0;
	while (data->args[i] && ft_strcmp(data->args[i], "|") != 0)
	{
		if (ft_strcmp(data->args[i], ">") == 0)
			cmd->output_file = data->args[++i];
		else if (ft_strcmp(data->args[i], ">>") == 0)
		{
			cmd->output_file = data->args[++i];
			cmd->is_append = 1;
		}
		else if (ft_strcmp(data->args[i], "<") == 0)
			cmd->input_file = data->args[++i];
		else if (ft_strcmp(data->args[i], "<<") == 0)
			cmd->input_file = data->args[++i];
		else
			cmd->args[arg_idx++] = ft_strdup(data->args[i]);
		i++;
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
			current_cmd->next = init_command(NULL, 0, 0);
			current_cmd = current_cmd->next;
		}
	}
}
/********************************************fill_command - end **************************************************/

/********************************************manage_env - start **************************************************/
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

char	*get_env_value(t_data *data, char *var_name)
{
	int		index;
	int		name_len;
	char	*var_string;

	index = find_env_var(data->envp, var_name);
	if (index == -1)
		return (NULL);
	var_string = data->envp[index];
	name_len = ft_strlen(var_name);
	if (var_string[name_len] == '=')
		return (&var_string[name_len + 1]);
	return (NULL);
}

int	env_len(char *str, int i)
{
	int	l;

	l = 0;
	if (str[i] == '?')
		return (1);
	while (str[i + l] && (ft_isalnum(str[i + l]) || str[i + l] == '_'))
		l++;
	return (l);
}

void	expand_and_copy(t_data *data, char *n_s, int *j, char *arg, int *i)
{
	char	*name;
	char	*value;
	int		name_len;

	(*i)++;
	name_len = env_len(arg, *i);
	name = ft_substr(arg, *i, name_len);
	value = get_env_value(data, name);
	free(name);
	if (value)
	{
		ft_strlcpy(&n_s[*j], value, ft_strlen(value) + 1);
		*j += ft_strlen(value);
	}
	*i += name_len;
}

int	get_expanded_len(t_data *data, char *str, int *i)
{
	char	*name;
	char	*value;
	int		len;
	int		name_len;

	(*i)++;
	name_len = env_len(str, *i);
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

char	*build_clean_arg(t_data *data, char *arg, int i, int j)
{
	char	*new_arg;
	char	quote_char;

	new_arg = malloc(sizeof(char) * (calculate_final_len(data, arg) + 1));
	if (!new_arg)
		return (NULL);
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
			expand_and_copy(data, new_arg, &j, arg, &i);
		else
			new_arg[j++] = arg[i++];
	}
	new_arg[j] = '\0';
	return (new_arg);
}

void	manage_env(t_data *data)
{
	int		i;
	char	*clean_arg;

	i = 0;
	while (data->args[i])
	{
		clean_arg = build_clean_arg(data, data->args[i], 0, 0);
		free(data->args[i]);
		data->args[i] = clean_arg;
		i++;
	}
}
/********************************************manage_env - end **************************************************/

/********************************************get args - start **************************************************/
int	get_arg_end(char *line, int i)
{
	char quote_char;

	quote_char = 0;
	while (line[i] && line[i] != ' ' && line[i] != '\t')
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			quote_char = line[i];
			i++;
			while (line[i] && line[i] != quote_char)
				i++;
            if (line[i] == quote_char)
                i++;
		}
		else
			i++;
	}
	return (i);
}

int nb_of_args(char *line)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while(line[i])
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (line[i])
		{
			count++;
			i = get_arg_end(line, i);
		}
	}
	return (count);
}

void get_args(t_data *data, char *line)
{
	int i;
	int j;
	int start;
	int arg_count;

	i = 0;
	j = 0;
	arg_count = nb_of_args(line);
	data->args = malloc(sizeof(char *) * (arg_count + 1));
	if(!data->args)
		return;
	while(j < arg_count)
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		start = i;
		i = get_arg_end(line, start);
		data->args[j] = ft_substr(line, start, i - start);
		j++;
	}
	data->args[j] = NULL;
}
/********************************************get args - end **************************************************/
void analyse_line(t_data *data, t_built_in *builtins, t_command *command, char *line)
{
	t_command *tmp;
	
	get_args(data, line); //Transforming the line we got from readline to args, and they stay like they are
	manage_env(data); //extending the args to their real value $USER will became cepheus
	fill_commands(data, command, 0); //moving each arg to the command structure, each command in one linked list while taking care and getting based on pipes and redirections
	tmp = command;
	while(tmp)
	{
		compare_cmd(data, tmp, builtins); //i should go from one linked list to another one and compare them to the builtins and execute them
		tmp = tmp->next;
	}
}
