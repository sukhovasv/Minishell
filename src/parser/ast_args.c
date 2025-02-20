/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:27:34 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:27:36 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(t_token *tokens)
{
	int		count;
	t_token	*curr;

	count = 0;
	curr = tokens;
	while (curr && curr->type != TOKEN_PIPE)
	{
		if (curr->type == TOKEN_WORD)
			count++;
		else if (is_redirect_token(curr->type))
			if (curr->next)
				curr = curr->next;
		curr = curr->next;
	}
	return (count);
}

char	**create_args_array(int count)
{
	return (ft_calloc(count + 1, sizeof(char *)));
}

int	copy_token_values(char **args, t_token *tokens, int count)
{
	int		i;
	t_token	*curr;

	i = 0;
	curr = tokens;
	while (curr && i < count)
	{
		if (curr->type == TOKEN_WORD)
		{
			args[i] = ft_strdup(curr->value);
			if (!args[i])
			{
				while (--i >= 0)
					free(args[i]);
				return (0);
			}
			i++;
		}
		curr = curr->next;
	}
	args[i] = NULL;
	return (1);
}

int	is_redirect_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HEREDOC);
}
