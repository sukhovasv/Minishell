/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:25:51 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:25:54 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_heredoc_token(const char *str, size_t len, int *expand_vars)
{
	t_token	*token;
	char	*cleaned_delim;

	token = new_token(TOKEN_WORD, str, len);
	if (!token)
		return (NULL);
	cleaned_delim = clean_delimiter(token->value);
	if (!cleaned_delim)
	{
		free_tokens(token);
		return (NULL);
	}
	*expand_vars = should_expand_heredoc_vars(str);
	free(token->value);
	token->value = cleaned_delim;
	return (token);
}
