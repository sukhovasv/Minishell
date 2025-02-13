#include "minishell.h"

void cleanup_heredoc_files(t_heredoc_data *heredocs, int count)
{
    int i;
	
	i = 0;
    if (!heredocs)
        return;
    while (i < count)
    {
        if (heredocs[i].temp_file)
        {
            unlink(heredocs[i].temp_file);
            free(heredocs[i].temp_file);
            heredocs[i].temp_file = NULL;
        }
        i++;
    }
    free(heredocs);
    heredocs = NULL;
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

