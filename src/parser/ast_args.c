#include "minishell.h"

int count_args(t_token *tokens)
{
    int count;
    t_token *curr;

    count = 0;
    curr = tokens;
    while (curr && curr->type != TOKEN_PIPE)
    {
        if (curr->type == TOKEN_WORD)
            count++;
        else if (is_redirect_token(curr->type))
            curr = curr->next;
        curr = curr->next;
    }
    return (count);
}

char **create_args_array(int count)
{
    return (ft_calloc(count + 1, sizeof(char *)));
}

int copy_token_values(char **args, t_token *tokens, int count)
{
    int i;
    t_token *curr;

    i = 0;
    curr = tokens;
    while (curr && i < count)
    {
        if (curr->type == TOKEN_WORD)
        {
            args[i] = ft_strdup(curr->value);
            if (!args[i])
                return (0);
            i++;
        }
        curr = curr->next;
    }
    args[i] = NULL;
    return (1);
}

void    free_args_array(char **args)
{
    int i;

    if (!args)
        return ;
    i = 0;
    while (args[i])
        free(args[i++]);
    free(args);
}
