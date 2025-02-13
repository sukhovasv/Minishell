/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dllist1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 16:33:13 by yberezhn          #+#    #+#             */
/*   Updated: 2024/07/15 16:33:16 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	dllist_foreach(t_dlnode *list,
			void (*f)(void *, void *), void *user_data)
{
	t_dlnode	*next;

	while (list)
	{
		next = list->next;
		(*f)((void *)(long)list->num, user_data);
		list = next;
	}
}

void	dllist_foreach_el(t_dlnode *list,
						void (*f)(void *, void *), void *user_data)
{
	t_dlnode	*next;

	while (list)
	{
		next = list->next;
		(*f)((void *)list, user_data);
		list = next;
	}
}

size_t	dllist_length(t_dlnode *node)
{
	size_t	len;

	len = 0;
	while (node)
	{
		node = node->next;
		len++;
	}
	return (len);
}

int	dllist_node_idx(t_dllist *list, t_dlnode *node)
{
	int			i;
	t_dlnode	*l_node;

	i = 0;
	l_node = list->head;
	while (l_node && l_node != node)
	{
		l_node = l_node->next;
		i++;
	}
	if (l_node != node)
		i = -1;
	return (i);
}
