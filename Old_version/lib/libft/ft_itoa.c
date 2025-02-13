/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 15:20:25 by yberezhn          #+#    #+#             */
/*   Updated: 2023/11/06 15:20:27 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_num_len(int n)
{
	int	len;

	len = 1;
	if (n < 0)
		len++;
	while (n / 10 != 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

static void	itoa_recursive(unsigned int n, char *buf)
{
	*buf = (n % 10) + '0';
	if (n / 10)
		itoa_recursive(n / 10, --buf);
}

char	*ft_itoa(int n)
{
	const int	len = get_num_len(n);
	char *const	num_str = ft_calloc(len + 1, sizeof(char));

	if (!num_str)
		return (NULL);
	if (n < 0)
	{
		num_str[0] = '-';
		itoa_recursive(-n, &num_str[len - 1]);
	}
	else
		itoa_recursive(n, &num_str[len - 1]);
	return (num_str);
}
