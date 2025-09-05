/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:01:13 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/05 18:28:39 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	if (n != 0)
	{
		while ((src[i] != '\0') && i < n - 1)
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (ft_strlen(src));
}

char	*ft_strdup(const char *s1)
{
	size_t		l;
	const char	*s2;

	l = ft_strlen(s1);
	s2 = malloc((sizeof(char) * l) + 1);
	if (s2 == NULL)
		return (NULL);
	ft_strlcpy((char *)s2, s1, l + 1);
	return ((char *)s2);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	l;
	size_t	n;

	if (!s)
		return (NULL);
	n = ft_strlen(s);
	if (start >= n)
		return (ft_strdup(""));
	if (start + len > n)
		l = n - start;
	else
		l = len;
	str = malloc((sizeof(char) * l) + 1);
	if (str == NULL)
		return (NULL);
	ft_strlcpy(((char *)str), (s + start), l + 1);
	return (str);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	size_t	l1;
	size_t	l2;
	char	*str;

	i = -1;
	j = -1;
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	str = malloc(sizeof(char) * (l1 + l2) + 1);
	if (str == NULL)
		return (NULL);
	while (++i < l1)
		str[i] = s1[i];
	while (i < (l1 + l2))
	{
		str[i] = s2[++j];
		i++;
	}
	str[i] = '\0';
	return (str);
}
