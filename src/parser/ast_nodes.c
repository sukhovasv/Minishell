#include "minishell.h"

t_ast_node	*create_command_node(char **args, t_redirect *redirects)
{
	t_ast_node	*node;

	node = ft_calloc(1, sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = AST_COMMAND;
	node->args = args;
	node->redirects = redirects;
	return (node);
}

t_ast_node	*create_pipe_node(t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = AST_PIPE;
	node->left = left;
	node->right = right;
	node->args = NULL;
	node->redirects = NULL;
	node->heredoc_data = NULL;
	node->pipe_level = 0;
	node->heredoc_count = 0;
	return (node);
}

t_ast_node	*create_redirect_node(t_ast_node *cmd, t_redirect *redir)
{
	t_ast_node	*node;

	node = ft_calloc(1, sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = AST_REDIRECT;
	node->left = cmd;
	node->redirects = redir;
	return (node);
}

t_redirect	*create_redirect(t_token_type type, const char *file,
		t_token *token)
{
	t_redirect	*redir;

	redir = ft_calloc(1, sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->token = token;
	redir->next = NULL;
	return (redir);
}

int	add_redirect_to_list(t_redirect **redirects, t_redirect *new_redir)
{
	t_redirect	*current;

	if (!new_redir)
		return (0);
	if (!*redirects)
		*redirects = new_redir;
	else
	{
		current = *redirects;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
	return (1);
}
