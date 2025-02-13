#include "minishell.h"

static int handle_export_error(char *arg)
{
    ft_putstr_fd("export: '", STDERR_FILENO);
    ft_putstr_fd(arg, STDERR_FILENO);
    ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
    return (1);
}

static int export_var(t_env *env, char *key, char *value)
{
    if (!is_valid_env_name(key))
        return (handle_export_error(key));
    
    if (!update_existing_variable(env, key, value))
        add_new_variable(env, key, value);
    return (0);
}

static char *get_key_value(char *arg, char **value)
{
    char *equal_sign;
    char *key;
    size_t key_len;
    size_t value_len;
    
    *value = NULL;
    equal_sign = ft_strchr(arg, '=');
    
    if (!equal_sign)
    {
        key = ft_strdup(arg);
        return key;
    }

    // Вычисляем длины ключа и значения
    key_len = equal_sign - arg;
    value_len = ft_strlen(equal_sign + 1);
    // Выделяем память для ключа
    key = (char *)malloc(key_len + 1);
    if (!key)
        return (NULL);
    // Копируем ключ
    ft_strlcpy(key, arg, key_len + 1);
    // Выделяем память и копируем значение
    *value = (char *)malloc(value_len + 1);
    if (!*value)
    {
        free(key);
        return (NULL);
    }
    ft_strlcpy(*value, equal_sign + 1, value_len + 1);
    return key;
}

int builtin_export(char **argv, t_env *env)
{
    int     i;
    char    *key;
    char    *value;
    int     status;

	if (!argv[1]) // Если `export` вызван без аргументов
    {
        print_sorted_env(env);
        return 0;
    }

    status = 0;
    i = 1;
    while (argv[i])
    {
        value = NULL;
        key = get_key_value(argv[i], &value);
        if (!key)
            return (1);
        if (export_var(env, key, value) != 0)
            status = 1;
        free(key);
        free(value);
        i++;
    }
    return (status);
}
