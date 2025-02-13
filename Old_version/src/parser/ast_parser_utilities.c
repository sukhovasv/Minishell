#include "minishell.h"

static void free_redirections(t_redirect *redirects)
{
    t_redirect *curr;
    t_redirect *next;

    curr = redirects;
    while (curr)
    {
        next = curr->next;
        free(curr->file);
        free(curr);
        curr = next;
    }
}

static void free_args(char **args)
{
    int i;

    i = 0;
    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
}

void free_ast_node(t_ast_node *node)
{
    if (!node)
        return;
    if (node->args)
        free_args(node->args);
    if (node->redirects)
        free_redirections(node->redirects);
    if (node->left)
        free_ast_node(node->left);
    if (node->right)
        free_ast_node(node->right);
    free(node);
}
