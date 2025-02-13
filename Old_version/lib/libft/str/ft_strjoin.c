/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 19:44:02 by yberezhn          #+#    #+#             */
/*   Updated: 2023/11/03 19:44:04 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	const size_t	size = ft_strlen(s1) + ft_strlen(s2) + 1;
	char *const		temp = malloc(sizeof(char) * size);

	if (!temp)
		return (NULL);
	ft_strlcpy(temp, s1, size);
	ft_strlcat(temp, s2, size);
	return (temp);
}
