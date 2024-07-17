/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 21:26:15 by yberezhn          #+#    #+#             */
/*   Updated: 2023/11/28 14:51:53 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	gnl_strlen(char *str)
{
	char *const	str_start = str;

	if (!str)
		return (0);
	while (*str)
		str++;
	return (str - str_start);
}

char	*gnl_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (1)
	{
		if (*s == (char)c)
			return ((char *)s);
		if (*s == '\0')
			return (NULL);
		s++;
	}
}

char	*gnl_strndup(char const *src, size_t n)
{
	char	*copy;
	size_t	len;
	size_t	i;

	len = 0;
	i = 0;
	while (len < n && src[len])
		len++;
	copy = (char *)malloc(sizeof(char) * len + 1);
	if (!copy)
		return (NULL);
	while (src && i < len)
	{
		copy[i] = src[i];
		i++;
	}
	copy[len] = '\0';
	return (copy);
}

char	*gnl_strjoin(char *s1, char *s2)
{
	const int	s1_len = gnl_strlen(s1);
	const int	s2_len = gnl_strlen(s2);
	char *const	str = (char *)malloc(sizeof(char) * (s1_len + s2_len + 1));
	size_t		i;

	if (!str)
		return (NULL);
	i = 0;
	while (*s1)
		str[i++] = *s1++;
	while (*s2)
		str[i++] = *s2++;
	str[s1_len + s2_len] = '\0';
	return (str);
}

char	*gnl_realloc_str(char *left_str, char *buff)
{
	char	*res;

	if (!buff)
		return (NULL);
	if (!left_str)
	{
		left_str = (char *)malloc(1 * sizeof(char));
		if (!left_str)
			return (NULL);
		left_str[0] = '\0';
	}
	res = gnl_strjoin(left_str, buff);
	free(left_str);
	return (res);
}
