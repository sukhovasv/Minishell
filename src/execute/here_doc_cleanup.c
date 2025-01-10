#include "minishell.h"

void cleanup_heredoc_files(t_heredoc_data *heredocs, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        unlink(heredocs[i].temp_file);
        free(heredocs[i].temp_file);
        i++;
    }
    free(heredocs);
}

int free_initialized_heredocs(t_heredoc_data *heredocs, int count)
{
    int j = 0;
    while (j < count)
    {
        unlink(heredocs[j].temp_file);
        free(heredocs[j].temp_file);
        j++;
    }
    return 0;
}
