/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 14:40:00 by yberezhn          #+#    #+#             */
/*   Updated: 2023/11/07 14:40:02 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;

	if (!ft_strlen(little) || big == little)
		return ((char *)big);
	while (len > 0 && *big)
	{
		i = 0;
		while (big[i] == little[i] && (len != i))
			if (!little[++i])
				return ((char *)big);
		big++;
		len--;
	}
	return (NULL);
}
