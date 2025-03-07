/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:27:43 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:27:45 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*build_ast(t_token *tokens)
{
	t_token		*current;
	t_ast_node	*ast;

	if (!tokens)
		return (NULL);
	current = tokens;
	ast = parse_pipe_sequence(&current);
	return (ast);
}

t_ast_node	*parse_simple_command(t_token **current)
{
	t_ast_node	*node;
	t_redirect	*redirects;
	char		**args;
	int			arg_count;
	t_token		*start;

	redirects = NULL;
	start = *current;
	if (!start)
		return (NULL);
	arg_count = count_args(start);
	args = create_args_array(arg_count);
	if (!args)
		return (NULL);
	if (!copy_token_values(args, start, arg_count))
	{
		free_args_array(args);
		return (NULL);
	}
	node = handle_command_redirects(current, args, &redirects);
	return (node);
}

static t_ast_node	*handle_pipe_sequence(t_token **current, t_ast_node *left)
{
	t_ast_node	*right;
	t_ast_node	*pipe_node;

	*current = (*current)->next;
	right = parse_pipe_sequence(current);
	if (!right)
	{
		free_ast_node(left);
		return (NULL);
	}
	pipe_node = create_pipe_node(left, right);
	if (!pipe_node)
	{
		free_ast_node(left);
		free_ast_node(right);
		return (NULL);
	}
	return (pipe_node);
}

t_ast_node	*parse_pipe_sequence(t_token **current)
{
	t_ast_node	*left;

	left = parse_simple_command(current);
	if (!left || !*current)
		return (left);
	if ((*current)->type == TOKEN_PIPE)
		return (handle_pipe_sequence(current, left));
	return (left);
}
