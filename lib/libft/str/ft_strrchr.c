/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 18:17:17 by yberezhn          #+#    #+#             */
/*   Updated: 2023/10/30 18:33:02 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strrchr(const char *s, int c)
{
	char	*res;

	res = NULL;
	while (1)
	{
		if (*s == (char)c)
			res = ((char *)s);
		if (*s == '\0')
			return (res);
		s++;
	}
}
