/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 19:26:08 by yberezhn          #+#    #+#             */
/*   Updated: 2023/12/06 17:35:45 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_char(int c)
{
	ft_putchar_fd(c, 1);
	return (1);
}

int	ft_print_str(char *str)
{
	char *const	null_str = "(null)";

	if (!str)
	{
		ft_putstr_fd(null_str, STDOUT_FILENO);
		return ((int)ft_strlen(null_str));
	}
	ft_putstr_fd(str, 1);
	return ((int)ft_strlen(str));
}

int	ft_print_p(va_list *args)
{
	const void		*ptr = va_arg(*args, void *);
	const uintptr_t	val = (uintptr_t) ptr;
	char			*res;
	int				len;

	if (!ptr)
		return (ft_print_str("(nil)"));
	res = NULL;
	len = ft_print_str("0x");
	res = ft_ultoa_base(val, "0123456789abcdef", 16);
	ft_putstr_fd(res, STDOUT_FILENO);
	len += ft_strlen(res);
	free(res);
	return (len);
}

int	ft_print_num(char flag, va_list *args)
{
	const char	*hex_base = "0123456789abcdef";
	const char	*hex_base_upper = "0123456789ABCDEF";
	char		*res;
	int			len;

	res = NULL;
	len = 0;
	if (flag == 'd' || flag == 'i')
		res = ft_itoa_base(va_arg(*args, int), "0123456789", 10);
	if (flag == 'u')
		res = ft_ultoa_base(va_arg(*args, unsigned int), "0123456789", 10);
	if (flag == 'x')
		res = ft_ultoa_base(va_arg(*args, unsigned int), hex_base, 16);
	if (flag == 'X')
		res = ft_ultoa_base(va_arg(*args, unsigned int), hex_base_upper, 16);
	if (!res)
		return (0);
	ft_putstr_fd(res, STDOUT_FILENO);
	len += ft_strlen(res);
	free(res);
	return (len);
}
