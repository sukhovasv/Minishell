#include "../include/minishell.h"

void initialize_pipe(int p[2])
{
    if (pipe(p) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
}

void read_and_write_heredoc(void)
{

}

void setup_heredoc_redirection(void)
{

}

int handle_heredoc_redirection(void)
{
    return (0);
}
