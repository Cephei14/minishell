/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:01:13 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 19:03:43 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	sign(const char *str, long int *i)
{
	if (str[*i] == '+' || str[*i] == '-')
	{
		(*i)++;
		if (!ft_isdigit((char)str[*i]))
		{
			errno = EINVAL;
			return (0);
		}
		if (str[*i - 1] == '-')
			return (-1);
	}
	return (1);
}

static int	lenn(const char *str, long int i)
{
	long int	l;

	l = 1;
	while (ft_isdigit(str[i]))
	{
		l = l * 10;
		i++;
	}
	return (l / 10);
}

static int	assign(const char *str, long int i, long int l)
{
	long int	n;

	n = 0;
	while (l)
	{
		n = n + (str[i] - '0') * l;
		i++;
		l /= 10;
	}
	return (n);
}

long long	ft_atoi(const char *str)
{
	long int	i;
	long int	n;
	long int	l;
	long int	result;

	i = 0;
	errno = 0;
	while (((str[i] >= 9 && str[i] <= 13) || (str[i] == 32)) && str[i])
		i++;
	n = sign(str, &i);
	if (n == 0)
		return (0);
	while (str[i] == '0' && str[i])
		i++;
	l = lenn(str, i);
	result = n * assign(str, i, l);
	if ((result > INT_MAX && n == 1) || (result < INT_MIN && n == -1))
	{
		errno = ERANGE;
		if (n == 1)
			return (INT_MAX);
		return (INT_MIN);
	}
	return ((int)result);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s)
		write(fd, s, ft_strlen(s));
}
