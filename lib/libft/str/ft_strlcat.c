/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 17:49:21 by yberezhn          #+#    #+#             */
/*   Updated: 2023/10/30 17:57:45 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	const char		*d = dest;
	const char		*s = src;
	size_t			n;
	size_t			dlen;

	n = size;
	while (n-- && *dest)
		dest++;
	dlen = dest - d;
	n = size - dlen;
	if (n == 0)
		return (dlen + ft_strlen(s));
	return (dlen + ft_strlcpy(dest, src, n));
}
