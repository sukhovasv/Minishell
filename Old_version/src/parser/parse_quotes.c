#include "minishell.h"

static char *extract_var_name(const char *input, int *i)
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

char *find_env_value(const char *var_name, int len, t_env *env)
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
}

static char *process_dollar(const char *input, int *i, t_env *env)
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

static void init_parse_state(int *i, int *j, int *in_single_quotes)
{
    *i = 0;
    *j = 0;
    *in_single_quotes = 0;
}

static char *allocate_result_buffer(const char *input)
{
    char *result;

    result = malloc(ft_strlen(input) * 2 + 1);
    if (!result)
        return (NULL);
    return (result);
}

static void handle_quotes(const char c, int *in_single_quotes)
{
    if (c == '\'')
        *in_single_quotes = !(*in_single_quotes);
}

static int handle_env_variable(const char *input, char *result, int *i, int *j, 
                             t_env *env, int in_single_quotes)
{
    char *env_val;

    if (input[*i] == '$' && !in_single_quotes)
    {
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

char *parse_quotes(const char *input, t_env *env)
{
    char *result;
    int i;
    int j;
    int in_single_quotes;

    result = allocate_result_buffer(input);
    if (!result)
        return (NULL);

    init_parse_state(&i, &j, &in_single_quotes);
    while (input[i])
    {
        handle_quotes(input[i], &in_single_quotes);
        if (handle_env_variable(input, result, &i, &j, env, in_single_quotes))
            continue;
        else
            result[j++] = input[i];
        i++;
    }
    result[j] = '\0';
    return (result);
}
