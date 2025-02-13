#include "minishell.h"

static char *process_value_with_quotes(char *value)
{
    size_t len;

	len = ft_strlen(value);
    if ((value[0] == '"' || value[0] == '\'') && value[len - 1] == value[0])
    {
        value[len - 1] = '\0';
        value++;
    }

    return value;
}

static char *process_value_without_quotes(char *value)
{
    if (!value || value[0] == '\0')
        return NULL;
    char *space_pos = strchr(value, ' ');
    if (space_pos)
        *space_pos = '\0';

    return value;
}

static char *process_value(char *value)
{
    if ((value[0] == '"' || value[0] == '\'') && value[strlen(value) - 1] == value[0])
        return process_value_with_quotes(value);
    return process_value_without_quotes(value);
}

char *create_env_string(const char *key, const char *value)
{
    if (!key || !value)
        return NULL;
    char *value_copy = ft_strdup(value);
    if (!value_copy)
        return NULL;
    char *clean_value = process_value(value_copy);
    if (!clean_value)
    {
        free(value_copy);
        return NULL;
    }
    size_t size = ft_strlen(key) + ft_strlen(clean_value) + 2;
    char *env_str = ft_calloc(size, sizeof(char));
    if (!env_str)
    {
        free(value_copy);
        return NULL;
    }
    ft_strlcpy(env_str, key, size);
    ft_strlcat(env_str, "=", size);
    ft_strlcat(env_str, clean_value, size);
    free(value_copy);
    return env_str;
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

size_t count_env_vars(char **env)
{
    size_t i;

    i = 0;
    while (env[i])
        i++;
    return (i);
}
