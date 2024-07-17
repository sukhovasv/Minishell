/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 19:26:19 by yberezhn          #+#    #+#             */
/*   Updated: 2023/12/02 19:26:21 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf(const char *s, ...)
{
	va_list	args;
	int		counter;

	counter = 0;
	va_start(args, s);
	while (*s)
	{
		if (*s != '%')
			counter += ft_print_char(*s);
		else
			counter += flag_handler(*++s, &args);
		s++;
	}
	va_end(args);
	return (counter);
}
