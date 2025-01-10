#include "minishell.h"

t_redirect *create_redirect(t_token_type type, const char *file)
{
    t_redirect *redir;

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
    redir->next = NULL;
    return (redir);
}

int add_redirect_to_list(t_redirect **redirects, t_redirect *new_redir)
{
    t_redirect *current;

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

int is_redirect_token(t_token_type type)
{
    return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
            type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HEREDOC);
}

void    free_redirects(t_redirect *redirects)
{
    t_redirect *current;
    t_redirect *next;

    current = redirects;
    while (current)
    {
        next = current->next;
        if (current->file)
            free(current->file);
        free(current);
        current = next;
    }
}