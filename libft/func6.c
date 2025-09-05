/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func6.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:30:02 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 18:31:07 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_intlen(int *n, int *s)
{
	int	i;
	int	l;

	if (*n >= 0)
		(*s) = 0;
	else
	{
		(*s) = 1;
		(*n) = -(*n);
	}
	l = 0;
	i = (*n);
	while (i > 0)
	{
		i = i / 10;
		l++;
	}
	if (*s == 1)
		return (l + 1);
	return (l);
}

static char	*limit(int n)
{
	if (n == 0)
		return ("0");
	else if (n == -2147483648)
		return ("-2147483648");
	return (NULL);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		i;
	int		len;
	int		s;

	if (limit(n))
		return (ft_strdup(limit(n)));
	len = ft_intlen(&n, &s);
	i = len;
	str = malloc((sizeof (char)) * len + 1);
	if (str == NULL)
		return (NULL);
	while (--i > -1)
	{
		str[i] = ((n % 10) + '0');
		n = n / 10;
	}
	if (s == 1)
		str[0] = '-';
	str[len] = '\0';
	return (str);
}
