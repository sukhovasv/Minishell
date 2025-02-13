/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dlnode.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 16:33:47 by yberezhn          #+#    #+#             */
/*   Updated: 2024/07/15 16:33:48 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_dlnode	*dlnode_alloc(int num)
{
	t_dlnode *const	node = ft_calloc(1, sizeof(t_dlnode));

	if (!node)
		return (NULL);
	node->num = num;
	return (node);
}

t_dllist	*dllist_append_node(t_dllist *list, t_dlnode *node)
{
	node->list = list;
	list->size++;
	if (list->size == 1)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		node->prev = list->tail;
		list->tail->next = node;
		list->tail = node;
	}
	return (list);
}

t_dllist	*dllist_prepend_node(t_dllist *list, t_dlnode *node)
{
	node->list = list;
	list->size++;
	if (list->size == 1)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		node->next = list->head;
		list->head->prev = node;
		list->head = node;
	}
	return (list);
}

//removes link and returns detached node
t_dlnode	*dllist_detach_node(t_dllist *list, t_dlnode *link)
{
	if (link == NULL || link->list != list)
		return (NULL);
	list->size--;
	link->list = NULL;
	if (link == list->head)
		list->head = list->head->next;
	if (link == list->tail)
		list->tail = list->tail->prev;
	if (link->prev && link->prev->next == link)
		link->prev->next = link->next;
	if (link->next && link->next->prev == link)
		link->next->prev = link->prev;
	link->next = NULL;
	link->prev = NULL;
	return (link);
}
