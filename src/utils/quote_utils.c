#include "minishell.h"

void init_quote_state(int *i, int *j, int *in_single_quotes, int *in_double_quotes)
{
    *i = 0;
    *j = 0;
    *in_single_quotes = 0;
    *in_double_quotes = 0;
}

char *allocate_result_buffer(const char *input)
{
    char *result;

    result = malloc(ft_strlen(input) * 2 + 1);
    if (!result)
        return (NULL);
    return (result);
}

void handle_quote(char c, int *in_single_quotes, int *in_double_quotes)
{
    if (c == '\'' && !*in_double_quotes)
        *in_single_quotes = !(*in_single_quotes);
    else if (c == '"' && !*in_single_quotes)
        *in_double_quotes = !(*in_double_quotes);
}

int handle_env_variable(const char *input, char *result, int *i, int *j, 
                       t_env *env, int in_single_quotes)
{
    char *env_val;

    if (input[*i] == '$' && !in_single_quotes)
    {
        if (input[*i + 1] == '?')
        {
            env_val = ft_itoa(env->last_status);
            (*i) += 2;
        }
        else
            env_val = handle_env_vars(input, i, env);

        if (env_val)
        {
            ft_strlcpy(&result[*j], env_val, ft_strlen(env_val) + 1);
            *j += ft_strlen(env_val);
            free(env_val);
        }
        return (1);
    }
    return (0);
}
