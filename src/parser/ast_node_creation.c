#include "minishell.h"

t_ast_node *create_command_node(char **args, t_redirect *redirects)
{
    t_ast_node *node;

    node = ft_calloc(1, sizeof(t_ast_node));
    if (!node)
        return (NULL);
    node->type = AST_COMMAND;
    node->args = args;
    node->redirects = redirects;
    return (node);
}

t_ast_node *create_pipe_node(t_ast_node *left, t_ast_node *right)
{
    t_ast_node *node;

    node = ft_calloc(1, sizeof(t_ast_node));
    if (!node)
        return (NULL);
    node->type = AST_PIPE;
    node->left = left;
    node->right = right;
    return (node);
}

t_ast_node *create_redirect_node(t_ast_node *cmd, t_redirect *redir)
{
    t_ast_node *node;

    node = ft_calloc(1, sizeof(t_ast_node));
    if (!node)
        return (NULL);
    node->type = AST_REDIRECT;
    node->left = cmd;
    node->redirects = redir;
    return (node);
}