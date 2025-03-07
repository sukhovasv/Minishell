/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 14:40:00 by yberezhn          #+#    #+#             */
/*   Updated: 2023/11/07 14:40:02 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *source, size_t n)
{
	const size_t	len = ft_strlen(source);
	char			*copy;

	if (!source)
		return (NULL);
	copy = ft_calloc(n + 1, sizeof(char));
	if (!copy)
		return (NULL);
	if (n > len)
		n = len;
	ft_strlcpy(copy, source, n + 1);
	return (copy);
}
