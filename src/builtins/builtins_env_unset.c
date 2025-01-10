#include "minishell.h"

void remove_variable(t_env *env_struct, const char *key)
{
    int i;
    int j;
    int key_len;

    i = 0;
    key_len = ft_strlen(key);
    while (env_struct->environ[i])
    {
        if (ft_strncmp(env_struct->environ[i], key, key_len) == 0 && 
            env_struct->environ[i][key_len] == '=')
        {
            free(env_struct->environ[i]);
            j = i;
            while (env_struct->environ[j])
            {
                env_struct->environ[j] = env_struct->environ[j + 1];
                j++;
            }
            return;
        }
        i++;
    }
}

int builtin_unset(char **argv, t_env *env_struct)
{
    int i;
    
    i = 1;
    while (argv[i])
    {
        if (ft_strchr(argv[i], '='))
        {
            ft_putstr_fd("unset: '", 2);
            ft_putstr_fd(argv[i], 2);
            ft_putendl_fd("': not a valid identifier", 2);
            return (1);
        }
        remove_variable(env_struct, argv[i]);
        i++;
    }
    return (0);
}

int builtin_env(char **environ)
{
    int i;

    i = 0;
    while (environ[i])
    {
        if (ft_strchr(environ[i], '='))  // Выводим только переменные со значением
        {
            ft_putendl_fd(environ[i], 1);
        }
        i++;
    }
    return (0);
}

int count_env_vars(char **env)
{
    int count;

    count = 0;
    while (env[count])
        count++;
    return (count);
}

char **allocate_new_environ(int env_size)
{
    char **new_environ;

    new_environ = ft_calloc(env_size + 2, sizeof(char *));  // +1 для новой переменной, +1 для NULL
    if (!new_environ)
        return (NULL);
    return (new_environ);
}

void copy_existing_vars(char **new_environ, char **old_environ, int env_size)
{
    int i;

    i = 0;
    while (i < env_size)
    {
        new_environ[i] = ft_strdup(old_environ[i]);
        i++;
    }
}

void add_new_variable(t_env *env_struct, const char *key, const char *value)
{
    int env_size;
    char **new_environ;
    char *new_var;
    size_t var_len;

    env_size = count_env_vars(env_struct->environ);
    new_environ = allocate_new_environ(env_size);
    if (!new_environ)
        return;

    copy_existing_vars(new_environ, env_struct->environ, env_size);

    // Создание новой переменной
    var_len = ft_strlen(key) + ft_strlen(value) + 2;  // +1 для '=' и +1 для '\0'
    new_var = ft_calloc(var_len, sizeof(char));
    if (!new_var)
    {
        free(new_environ);
        return;
    }

    ft_strlcpy(new_var, key, var_len);
    ft_strlcat(new_var, "=", var_len);
    ft_strlcat(new_var, value, var_len);
    new_environ[env_size] = new_var;
    new_environ[env_size + 1] = NULL;

    // Замена старого environ
    free(env_struct->environ);
    env_struct->environ = new_environ;
}