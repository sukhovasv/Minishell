/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 19:43:33 by yberezhn          #+#    #+#             */
/*   Updated: 2023/11/03 19:43:34 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	const size_t	s_len = ft_strlen(&s[start]);
	char			*buf;

	if (!s || ft_strlen(s) <= start)
		return (ft_calloc(1, 1));
	if (s_len < len)
		len = s_len;
	buf = ft_calloc(len + 1, sizeof(char));
	if (!buf)
		return (NULL);
	ft_strlcpy(buf, &s[start], len + 1);
	return (buf);
}
