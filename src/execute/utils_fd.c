#include "minishell.h"

void    restore_fds(int saved_stdout, int saved_stdin)
{
    if (saved_stdout != -1)
    {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }
    if (saved_stdin != -1)
    {
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdin);
    }
}

int     handle_command_error(t_ast_node *cmd, int saved_stdout, int saved_stdin)
{
    if (cmd)
        free_ast_node(cmd);
    restore_fds(saved_stdout, saved_stdin);
    return (1);
}