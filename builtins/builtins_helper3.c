/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_helper3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:46:51 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/03 20:54:20 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_value(char *str, int i)
{
	int l;

	l = ft_strlen(str) - i;
	return (ft_substr(str, i, l));
}

char *get_name(char *str, int *i)
{
	if((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'Z') || str[0] == '_')
	{
		while(str[*i])
		{
			if(str[*i] == '=')
				return (ft_substr(str, 0, *i));
			(*i)++;
		}
	}
	else
		return (NULL);
}