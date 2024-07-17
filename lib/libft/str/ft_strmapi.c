/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:49:25 by yberezhn          #+#    #+#             */
/*   Updated: 2023/11/06 17:49:27 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char *const		buf = ft_strdup(s);

	i = 0;
	if (!buf)
		return (NULL);
	while (buf[i])
	{
		buf[i] = (*f)(i, buf[i]);
		i++;
	}
	return (buf);
}
