/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_reallocarray.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 18:10:45 by yberezhn          #+#    #+#             */
/*   Updated: 2023/10/30 18:15:52 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_reallocarray(void *ptr,
					size_t oldnmemb, size_t newnmemb, size_t size)
{
	void			*new_ptr;
	const size_t	newsize = newnmemb * size;
	const size_t	oldsize = oldnmemb * size;

	if (!ptr)
	{
		ptr = ft_calloc(newnmemb, size);
		if (!ptr)
			return (NULL);
		return (ptr);
	}
	if ((oldsize / size) != oldnmemb || ((newsize / size) != newnmemb))
		return (NULL);
	new_ptr = ft_calloc(newnmemb, size);
	if (!new_ptr)
	{
		free(ptr);
		return (NULL);
	}
	if (newsize > oldsize)
		ft_memcpy(new_ptr, ptr, oldsize);
	else
		ft_memcpy(new_ptr, ptr, newsize);
	free(ptr);
	return (new_ptr);
}
