/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 23:04:35 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/04 23:25:10 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	copy_env_value(t_data *data, char *new_arg, int *x, char *env_name)
{
	char	*env_value;
	int		j;

	env_value = get_env_value(data, env_name);
	j = 0;
	if (env_value)
	{
		while (env_value[j])
		{
			new_arg[*x] = env_value[j];
			(*x)++;
			j++;
		}
	}
}

int main(int ac, char **av, char **envp)
{
	char *line;
	t_data *data;

	(void)ac;
	(void)av;
	data = init_data(envp, 0, 0); //Here we create data structure and make a copy of the original environment variables that we have, you can write "export" in your shell to see them.
	while(1) //Infinite loop to keep waiting the commands.
	{
		line = readline("minishell> "); //(char *line) will have what we worte in the shell. (echo "abc" "def" $HOME -> || line = echo "abc" "def" $HOME ||)
		if (line == NULL)
		{
			if (isatty(STDIN_FILENO))
				printf("exit\n");
			break;
		}
		if (*line)
			add_history(line);
		if (data->args)
            free_split(data->args);
		declare(data, line); //Here we create the struct built_in and link each command with it's function. 
		free(line);
	}
	free_data(data);
	return 0;
}
