#include "minishell.h"

static int handle_heredoc_redirections(t_redirect *redirects, t_fd_info *fd_info)
{
    t_redirect *current = redirects;
    int status;

    while (current)
    {
        if (current->type == TOKEN_REDIR_HEREDOC)
        {
            status = handle_redir_heredoc(redirects, fd_info);
            if (!status)
                return 0;
            break;
        }
        current = current->next;
    }
    return 1;
}

static int handle_non_heredoc_redirections(t_redirect *redirects, t_fd_info *fd_info)
{
    t_redirect *current = redirects;
    int status;

    while (current)
    {
        if (current->type != TOKEN_REDIR_HEREDOC)
        {
            if (current->type == TOKEN_REDIR_IN)
                status = handle_redir_input(current, &fd_info->saved_stdin);
            else if (current->type == TOKEN_REDIR_OUT)
                status = handle_redir_output(current, &fd_info->saved_stdout);
            else if (current->type == TOKEN_REDIR_APPEND)
                status = handle_redir_append(current, &fd_info->saved_stdout);

            if (!status)
                return 0;
        }
        current = current->next;
    }
    return 1;
}

int handle_redirections(t_redirect *redirects, t_fd_info *fd_info)
{
    fd_info->saved_stdout = -1;
    fd_info->saved_stdin = -1;

    if (!handle_heredoc_redirections(redirects, fd_info))
        return (0);
    if (!handle_non_heredoc_redirections(redirects, fd_info))
        return (0);
    return (1);
}
