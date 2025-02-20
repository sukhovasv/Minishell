/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 14:40:00 by yberezhn          #+#    #+#             */
/*   Updated: 2023/11/07 14:40:02 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strncpy(char *dst, const char *src, unsigned int n)
{
	char *const	dest = dst;
	const char	*s = src;

	while (n-- != 0)
	{
		*dst++ = *s;
		if (*s++ == 0)
		{
			while (n-- != 0)
				*dst++ = '\0';
			break ;
		}
	}
	return (dest);
}
