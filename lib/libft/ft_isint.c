/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isint.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 19:43:09 by yberezhn          #+#    #+#             */
/*   Updated: 2023/11/03 19:43:18 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	is_int(char *arg)
{
	long long	num;
	int			sign;

	sign = 1;
	if (!*arg)
		return (false);
	if (*arg == '-' || *arg == '+')
	{
		if (*arg++ == '-')
			sign = -1;
		if (!*arg)
			return (false);
	}
	num = 0;
	while (*arg)
	{
		if (!ft_isdigit(*arg))
			return (false);
		num = num * 10 + (*arg - '0');
		if (((sign == 1 && num > INT_MAX) || (sign == -1 && - num < INT_MIN)))
			return (false);
		arg++;
	}
	return (true);
}
