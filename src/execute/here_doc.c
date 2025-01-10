#include "minishell.h"

int write_single_heredoc(t_heredoc_data *data)
{
    int fd = open_temp_file(data->temp_file);
    if (fd == -1)
        return -1;

    if (read_and_write_lines(fd, data) == -1)
    {
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

int handle_signals_and_fork(t_heredoc_data *heredocs, int count)
{
    int status;

    ignore_signals();
    status = fork_and_process_heredocs(heredocs, count);
    restore_signals();

    return (status != -1 && WIFEXITED(status) && WEXITSTATUS(status) == 0);
}

int handle_redir_heredoc(t_redirect *redir, t_fd_info *fd_info)
{
    t_heredoc_data *heredocs;
    int count;
    int last_fd;

    count = prepare_heredoc_data(redir, &heredocs);
    if (count == 0)
        return 0;
    last_fd = process_and_open_last_heredoc(heredocs, count);
    if (last_fd == -1)
        return 0;
    setup_input_redirection(last_fd, fd_info);
    return 1;
}
