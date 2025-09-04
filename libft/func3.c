/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdhaibi <rdhaibi@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:01:13 by rdhaibi           #+#    #+#             */
/*   Updated: 2025/09/03 21:23:25 by rdhaibi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalnum(char c)
{
	if (ft_isdigit(c) || ft_isalpha(c))
		return (1);
	else
		return (0);
}

static char	*edges(char const *s, char c, int *i)
{
	int	l1;
	int	l2;

	while (s[*i] == c)
		(*i)++;
	l1 = *i;
	while (s[*i] != '\0' && s[*i] != c)
		(*i)++;
	l2 = *i;
	return (ft_substr(s, l1, l2 - l1));
}

static int	wordsnum(char const *s, char c)
{
	int	n;

	n = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			n++;
			while (*s && *s != c)
				s++;
		}
	}
	return (n);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	char	**str;

	if (!s)
		return (NULL);
	str = malloc ((sizeof(char *)) * (wordsnum(s, c) + 1));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (j < wordsnum(s, c))
	{
		str[j] = edges(s, c, &i);
		if (!str[j])
		{
			while (j--)
				free(str[j]);
			free(str);
			return (NULL);
		}
		j++;
	}
	str[j] = NULL;
	return (str);
}
