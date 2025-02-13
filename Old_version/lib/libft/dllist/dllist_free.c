/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dllist_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 16:33:38 by yberezhn          #+#    #+#             */
/*   Updated: 2024/07/15 16:33:41 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_node(void *node, void *user_data)
{
	(void)user_data;
	free((t_dlnode *)node);
}

t_dlnode	*dllist_free(t_dlnode *list)
{
	dllist_foreach_el(list, free_node, NULL);
	return (list);
}
