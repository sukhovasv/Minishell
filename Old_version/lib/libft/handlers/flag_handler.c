/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 19:25:56 by yberezhn          #+#    #+#             */
/*   Updated: 2023/12/02 19:25:59 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	flag_handler(char flag, va_list *args)
{
	int	res;

	res = 0;
	if (flag == 'c')
		res += ft_print_char(va_arg(*args, int));
	else if (flag == 's')
		res += ft_print_str(va_arg(*args, char *));
	else if (flag == 'p')
		res += ft_print_p(args);
	else if (flag == '%')
		res += ft_print_char(flag);
	else if (ft_strchr("diuxX", flag))
		res += ft_print_num(flag, args);
	return (res);
}
