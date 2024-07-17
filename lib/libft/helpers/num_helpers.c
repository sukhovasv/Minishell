/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   num_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 19:26:04 by yberezhn          #+#    #+#             */
/*   Updated: 2023/12/02 19:26:05 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	unsigned_num_len(unsigned long num, int radix)
{
	int	len;

	len = 1;
	while (num / radix)
	{
		len++;
		num /= radix;
	}
	return (len);
}

static int	get_num_len(long num, int radix)
{
	int	len;

	len = 1;
	if (num < 0)
		len++;
	while (num / radix)
	{
		len++;
		num /= radix;
	}
	return (len);
}

static void	itoa_iterative_base(
		unsigned int n, char *buf, const char *base, int radix)
{
	*buf-- = base[(n % radix)];
	while (n / radix)
	{
		n = n / radix;
		*buf-- = base[(n % radix)];
	}
}

char	*ft_itoa_base(int n, const char *base, int radix)
{
	const int	len = get_num_len(n, radix);
	char *const	num_str = ft_calloc(len + 1, sizeof(char));

	if (!num_str)
		return (NULL);
	if (n < 0)
	{
		num_str[0] = '-';
		itoa_iterative_base(-n, &num_str[len - 1], base, radix);
	}
	else
		itoa_iterative_base(n, &num_str[len - 1], base, radix);
	return (num_str);
}

char	*ft_ultoa_base(unsigned long n, const char *base, int radix)
{
	const int	len = unsigned_num_len(n, radix);
	char *const	num_str = ft_calloc(len + 1, sizeof(char));
	int			i;

	if (!num_str)
		return (NULL);
	i = 1;
	num_str[len - i++] = base[(n % radix)];
	while (n / radix)
	{
		n = n / radix;
		num_str[len - i] = base[(n % radix)];
		i++;
	}
	return (num_str);
}
