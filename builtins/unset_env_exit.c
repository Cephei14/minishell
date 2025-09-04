/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_env_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:46:51 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/04 16:08:32 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int unset(t_data *data)
{
	(void)data;
	printf("this is unset\n");
    return (0);
}

int env(t_data *data)
{
	(void)data;
	printf("this is env\n");
    return (0);
}

int b_exit(t_data *data)
{
	(void)data;
	printf("this is exit\n");
    return (0);
}
