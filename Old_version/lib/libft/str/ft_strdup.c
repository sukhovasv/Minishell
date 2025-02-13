/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 19:38:18 by yberezhn          #+#    #+#             */
/*   Updated: 2023/10/30 19:40:11 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strdup(const char *src)
{
	char		*copy;
	const int	length = ft_strlen(src);

	copy = malloc(sizeof(char) * length + 1);
	if (!copy)
		return (NULL);
	ft_strlcpy(copy, (char *)src, length + 1);
	return (copy);
}
