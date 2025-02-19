/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:21:58 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:22:00 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*process_value_with_quotes(char *value)
{
	size_t	len;
	char	*new_value;

	len = ft_strlen(value);
	if (len < 2 || value[0] != value[len - 1])
		return (ft_strdup(value));
	new_value = ft_substr(value, 1, len - 2);
	return (new_value);
}

static char	*process_value_without_quotes(char *value)
{
	char	*space_pos;
	char	*new_value;

	if (!value || value[0] == '\0')
		return (NULL);
	space_pos = ft_strchr(value, ' ');
	if (space_pos)
		*space_pos = '\0';
	new_value = ft_strdup(value);
	return (new_value);
}

char	*process_value(char *value)
{
	if (!value || value[0] == '\0')
		return (NULL);
	if ((value[0] == '"' || value[0] == '\'')
		&& value[ft_strlen(value) - 1] == value[0])
		return (process_value_with_quotes(value));
	return (process_value_without_quotes(value));
}
