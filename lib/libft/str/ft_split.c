/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 19:43:09 by yberezhn          #+#    #+#             */
/*   Updated: 2023/11/03 19:43:18 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static char	**malloc_error(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	return (NULL);
}

int	word_count(char const *str, char c)
{
	int			i;
	int			word_count;

	i = 0;
	word_count = 0;
	while (str[i])
	{
		while (str[i] == c)
			i++;
		if (str[i] != c && str[i])
		{
			word_count++;
			while (str[i] != c && str[i])
				i++;
		}
	}
	return (word_count);
}

int	get_next_word_index(char const *str, char c)
{
	int	i;

	i = 0;
	while (str[i] == c)
		i++;
	return (i);
}

char	*get_word(char const *str, char c, int *start)
{
	char	*new;
	int		len;

	len = 0;
	*start += get_next_word_index(&str[*start], c);
	while (str[*start + len] && str[*start + len] != c)
		len++;
	if (len <= 0)
		return (NULL);
	new = ft_substr(str, *start, len);
	*start += len;
	return (new);
}

char	**ft_split(char const *str, char c)
{
	const int	wc = word_count(str, c);
	char		**arr;
	int			string_index;
	int			word_index;

	string_index = 0;
	word_index = 0;
	if (!str)
		return (NULL);
	arr = ft_calloc(wc + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	while (word_index < wc)
	{
		arr[word_index] = get_word(str, c, &string_index);
		if (!arr[word_index])
			return (malloc_error(arr));
		word_index++;
	}
	return (arr);
}
