#include "minishell.h"

size_t get_env_var_size(const char *key, const char *value)
{
    return (ft_strlen(key) + ft_strlen(value) + 2);
}

char *create_env_string(const char *key, const char *value)
{
    char *env_str;
    size_t size;

    size = get_env_var_size(key, value);
    env_str = ft_calloc(size, sizeof(char));
    if (!env_str)
        return (NULL);

    ft_strlcpy(env_str, key, size);
    ft_strlcat(env_str, "=", size);
    ft_strlcat(env_str, value, size);
    return (env_str);
}

int is_valid_env_name(const char *str)
{
    int i;

    if (!str || !ft_isalpha(str[0]))
        return (0);
    i = 1;
    while (str[i])
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

size_t count_env_size(char **env)
{
    size_t i;

    i = 0;
    while (env[i])
        i++;
    return (i);
}