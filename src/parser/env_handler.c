#include "minishell.h"

static int get_env_var_length(const char *str, size_t *i)
{
    int len = 0;
    while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
    {
        (*i)++;
        len++;
    }
    return len;
}

static char *expand_special_var(const char *str, size_t *i, t_env *env)
{
    if (str[*i] == '?')
    {
        (*i)++;
        return ft_itoa(env->last_status);
    }
    return NULL;
}

static char *get_env_var_value(const char *str, size_t start, int len)
{
    char *var_name = ft_strndup(&str[start], len);
    char *value = getenv(var_name);
    free(var_name);

    if (value)
        return ft_strdup(value);
    return ft_strdup("");
}

char *expand_env_var(const char *str, size_t *i, t_env *env)
{
    char *special_var;
    int len;
    size_t start;

    special_var = expand_special_var(str, i, env);
    if (special_var)
        return special_var;
    (*i)++; // Пропускаем $
    start = *i;
    len = get_env_var_length(str, i);
    if (len == 0)
        return ft_strdup("$");
    return get_env_var_value(str, start, len);
}
