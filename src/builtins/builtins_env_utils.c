#include "minishell.h"

int update_existing_variable(t_env *env, char *key, char *value)
{
    int     i;
    int     key_len;
    char    *new_var;
    
    i = 0;
    key_len = ft_strlen(key);
    while (env->environ[i])
    {
        if (ft_strncmp(env->environ[i], key, key_len) == 0 
            && env->environ[i][key_len] == '=')
        {
            new_var = create_env_string(key, value);
            if (!new_var)
                return (0);
            free(env->environ[i]);
            env->environ[i] = new_var;
            return (1);
        }
        i++;
    }
    return (0);
}

void add_new_variable(t_env *env, char *key, char *value)
{
    char    *new_var;
    
    new_var = create_env_string(key, value);
    if (!new_var)
        return;
    if (!add_env_var(env, new_var))
    {
        free(new_var);
        return;
    }
}

int add_env_var(t_env *env, char *new_var)
{
    int     i;
    char    **new_environ;
    
    i = count_env_size(env->environ);
    new_environ = ft_calloc(i + 2, sizeof(char *));
    if (!new_environ)
        return (0);
    i = 0;
    while (env->environ[i])
    {
        new_environ[i] = ft_strdup(env->environ[i]);
        if (!new_environ[i])
        {
            while (--i >= 0)
                free(new_environ[i]);
            free(new_environ);
            return (0);
        }
        i++;
    }
    new_environ[i] = new_var;
    new_environ[i + 1] = NULL;
    i = 0;
    while (env->environ[i])
    {
        free(env->environ[i]);
        i++;
    }
    free(env->environ);
    env->environ = new_environ;
    return (1);
}

t_env *init_env(char **system_environ)
{
    t_env *env;
    int i;

    if (!system_environ)  // Проверка входного параметра
        return (NULL);
    env = (t_env *)malloc(sizeof(t_env));
    if (!env)
        return (NULL);
    // Подсчитываем количество переменных
    i = 0;
    while (system_environ[i])
        i++;
    // Выделяем память под новый массив
    env->environ = (char **)malloc(sizeof(char *) * (i + 1));
    if (!env->environ)
    {
        free(env);
        return (NULL);
    }
    // Копируем каждую строку
    i = 0;
    while (system_environ[i])
    {
        env->environ[i] = ft_strdup(system_environ[i]);
        if (!env->environ[i])
        {
            // В случае ошибки освобождаем всю выделенную память
            while (--i >= 0)
                free(env->environ[i]);
            free(env->environ);
            free(env);
            return (NULL);
        }
        i++;
    }
    env->environ[i] = NULL;
    env->last_status = 0;  // Инициализируем статус последней команды
    return (env);
}

void free_env(t_env *env)
{
    int i;

    if (!env)
        return;

    if (env->environ)
    {
        i = 0;
        while (env->environ[i])
        {
            free(env->environ[i]);
            i++;
        }
        free(env->environ);
    }
    free(env);
}