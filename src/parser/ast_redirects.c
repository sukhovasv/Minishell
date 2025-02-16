#include "minishell.h"

t_ast_node	*handle_command_redirects(t_token **current, char **args,
		t_redirect **redirects)
{
	t_ast_node		*node;
	t_token_type	type;

	while (*current && (*current)->type != TOKEN_PIPE)
	{
		if (is_redirect_token((*current)->type))
		{
			type = (*current)->type;
			*current = (*current)->next;
			if (!process_redirect(current, type, redirects))
			{
				cleanup_command(args, *redirects);
				return (NULL);
			}
			continue ;
		}
		*current = (*current)->next;
	}
	node = create_command_node(args, *redirects);
	if (!node)
		cleanup_command(args, *redirects);
	return (node);
}

t_ast_node	*create_pipe_sequence(t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*pipe_node;

	pipe_node = create_pipe_node(left, right);
	if (!pipe_node)
	{
		free_ast_node(left);
		free_ast_node(right);
		return (NULL);
	}
	return (pipe_node);
}

int	process_redirect(t_token **current, t_token_type type,
		t_redirect **redirects)
{
	t_redirect	*new_redirect;
	int			result;

	if (!*current || (*current)->type != TOKEN_WORD)
		return (0);
	new_redirect = create_redirect(type, (*current)->value, *current);
	result = add_redirect_to_list(redirects, new_redirect);
	return (result);
}

void	cleanup_command(char **args, t_redirect *redirects)
{
	free_args_array(args);
	free_redirects(redirects);
}
