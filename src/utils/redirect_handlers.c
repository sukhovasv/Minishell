#include "minishell.h"

int handle_redir_input(t_redirect *redir, int *saved_stdin)
{
    int fd;

    fd = open(redir->file, O_RDONLY);
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(redir->file, 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        return (0);
    }
    if (*saved_stdin == -1)
        *saved_stdin = dup(STDIN_FILENO);
    dup2(fd, STDIN_FILENO);
    close(fd);
    return (1);
}

int handle_redir_output(t_redirect *redir, int *saved_stdout)
{
    int fd;

    fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(redir->file, 2);
        ft_putstr_fd(": Permission denied\n", 2);
        return (0);
    }
    if (*saved_stdout == -1)
        *saved_stdout = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return (1);
}

int handle_redir_append(t_redirect *redir, int *saved_stdout)
{
    int fd;

    fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(redir->file, 2);
        ft_putstr_fd(": Permission denied\n", 2);
        return (0);
    }
    if (*saved_stdout == -1)
        *saved_stdout = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return (1);
}

void restore_redirections(t_fd_info *fd_info)
{
    if (fd_info->saved_stdin != -1)
    {
        dup2(fd_info->saved_stdin, STDIN_FILENO);
        close(fd_info->saved_stdin);
        fd_info->saved_stdin = -1;
    }
    // Нужно добавить восстановление stdout
    if (fd_info->saved_stdout != -1)
    {
        dup2(fd_info->saved_stdout, STDOUT_FILENO);
        close(fd_info->saved_stdout);
        fd_info->saved_stdout = -1;
    }
}

int handle_heredoc_redirections(t_redirect *redirects, t_fd_info *fd_info, t_env *env, t_ast_node *ast)
{
    t_redirect *current = redirects;
    int status;

    while (current)
    {
        if (current->type == TOKEN_REDIR_HEREDOC)
        {
            status = handle_redir_heredoc(redirects, fd_info, env, ast);
            if (!status)
                return 0;
            break;
        }
        current = current->next;
    }
    return 1;
}

int handle_non_heredoc_redirections(t_redirect *redirects, t_fd_info *fd_info)
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

int handle_redirections(t_redirect *redirects, t_fd_info *fd_info, t_env *env, t_ast_node *ast)
{
    fd_info->saved_stdout = -1;
    fd_info->saved_stdin = -1;

    if (!handle_heredoc_redirections(redirects, fd_info, env, ast))
        return (0);
    if (!handle_non_heredoc_redirections(redirects, fd_info))
        return (0);
    return (1);
}
