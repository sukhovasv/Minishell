#include "minishell.h"

char *extract_var_name(const char *input, int *i)
{
    int start = *i;
    int len = 0;

    while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
    {
        len++;
        (*i)++;
    }

    return ft_strndup(&input[start], len);
}

/*char *find_env_value(const char *var_name, int len, t_env *env)
{
    int j;

    j = 0;
    while (env->environ[j])
    {
        if (ft_strncmp(env->environ[j], var_name, len) == 0 && env->environ[j][len] == '=')
            return (ft_strdup(env->environ[j] + len + 1));
        j++;
    }
    return (ft_strdup(""));
}*/

char *process_dollar(const char *input, int *i, t_env *env)
{
    (*i)++;

    char *var_name = extract_var_name(input, i);
    if (!var_name)
        return NULL;

    char *value = find_env_value(var_name, ft_strlen(var_name), env);
    free(var_name);

    return value;
}

char *handle_env_vars(const char *input, int *i, t_env *env)
{
    if (input[*i] == '$')
    {
        return process_dollar(input, i, env);
    }
    return ft_strdup("");
}

char *parse_quotes(const char *input, t_env *env)
{
    char *result;
    int i, j;
    int in_single_quotes, in_double_quotes;

    result = allocate_result_buffer(input);
    if (!result)
        return (NULL);

    init_quote_state(&i, &j, &in_single_quotes, &in_double_quotes);
    while (input[i])
    {
        if ((input[i] == '\'' && !in_double_quotes) ||
            (input[i] == '"' && !in_single_quotes))
        {
            handle_quote(input[i], &in_single_quotes, &in_double_quotes);
            i++;
            continue;
        }
        if (handle_env_variable(input, result, &i, &j, env, in_single_quotes))
            continue;
        else
            result[j++] = input[i++];
    }
    result[j] = '\0';
    return (result);
}
