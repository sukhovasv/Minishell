#include "minishell.h"

int allocate_heredoc_data(int count, t_heredoc_data **heredocs)
{
    *heredocs = malloc(sizeof(t_heredoc_data) * count);
    if (!*heredocs)
        return 0;
    return 1;
}

int initialize_single_heredoc(t_heredoc_data *heredoc, t_redirect *current)
{
    heredoc->temp_file = create_temp_file();
    heredoc->delimiter = current->file;
    if (!heredoc->temp_file)
        return 0;
    return 1;
}

int initialize_heredocs(t_redirect *redir, t_heredoc_data *heredocs)
{
    t_redirect *current = redir;
    int i = 0;

    while (current)
    {
        if (current->type == TOKEN_REDIR_HEREDOC)
        {
            if (!process_current_heredoc(heredocs, current, i))
                return 0;
            i++;
        }
        current = current->next;
    }
    return 1;
}

static int count_heredocs(t_redirect *redir)
{
    int count = 0;
    t_redirect *current = redir;

    while (current)
    {
        if (current->type == TOKEN_REDIR_HEREDOC)
            count++;
        current = current->next;
    }
    return count;
}

int prepare_heredoc_data(t_redirect *redir, t_heredoc_data **heredocs)
{
    int count = count_heredocs(redir);

    if (!allocate_heredoc_data(count, heredocs))
        return 0;

    if (!initialize_heredocs(redir, *heredocs))
    {
        free(*heredocs);
        return 0;
    }

    return count;
}
