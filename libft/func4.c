/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:01:13 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/04 18:42:55 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char )c)
			return ((char *)s + i);
		i++;
	}
	if ((char)c == '\0')
		return ((char *)s + i);
	return (NULL);
}

static int	occurence(char const *s1, char const *set, int l, char c)
{
	int	i;

	i = 0;
	if (c == '+')
	{
		while (s1[i] != '\0' && ft_strchr(set, s1[i]))
			i++;
		return (i);
	}
	if (c == '-')
	{
		while (ft_strchr(set, s1[l - 1]))
			l--;
		return (l);
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int	l;
	int	begin;
	int	end;

	if (!s1)
		return (NULL);
	l = ft_strlen((char *) s1);
	begin = occurence(s1, set, l, '+');
	if (begin == l)
		return (ft_strdup(""));
	end = occurence(s1, set, l, '-');
	return (ft_substr(s1, begin, end - begin));
}
