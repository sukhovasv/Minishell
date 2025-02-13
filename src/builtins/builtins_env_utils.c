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
    char *new_var = create_env_string(key, value);
    if (!new_var)
        return;
    if (!add_env_var(env, new_var))
        free(new_var); // Освобождаем память, если не удалось добавить
}

char **allocate_new_environ(char **old_environ, int additional_size)
{
    int i;
	char **new_environ;
	
	i = count_env_vars(old_environ);
	new_environ = ft_calloc(i + additional_size + 1, sizeof(char *)); // +1 для NULL
    if (!new_environ)
        return NULL;

    for (int j = 0; j < i; j++)
    {
        new_environ[j] = ft_strdup(old_environ[j]);
        if (!new_environ[j])  // Ошибка выделения памяти
        {
            while (--j >= 0)
                free(new_environ[j]);
            free(new_environ);
            return NULL;
        }
    }
    return new_environ;
}

int add_env_var(t_env *env, char *new_var)
{
    char **new_environ = allocate_new_environ(env->environ, 1);  // Выделяем память для +1 переменной
    if (!new_environ)
        return 0;

    int i = count_env_vars(env->environ);
    new_environ[i] = new_var;  // Просто добавляем указатель (без ft_strdup)
    new_environ[i + 1] = NULL; // Завершаем массив

    // Освобождаем старый `environ`
    for (int j = 0; env->environ[j]; j++)
        free(env->environ[j]);
    free(env->environ);

    env->environ = new_environ;
    return 1;
}


t_env *init_env(char **environ)
{
    t_env *env;
    int i;
    int size;

    ft_putstr_fd("\n[DEBUG] Initializing environment\n", 2);
    
    size = 0;
    while (environ[size])
        size++;

    ft_putstr_fd("[DEBUG] Environment size: ", 2);
    char size_str[20];
    sprintf(size_str, "%d", size);
    ft_putstr_fd(size_str, 2);
    ft_putstr_fd("\n", 2);

    env = malloc(sizeof(t_env));
    if (!env)
        return NULL;

    env->environ = malloc(sizeof(char *) * (size + 1));
    if (!env->environ)
    {
        free(env);
        return NULL;
    }

    i = 0;
    while (i < size)
    {
        env->environ[i] = ft_strdup(environ[i]);
        if (!env->environ[i])
        {
            while (i > 0)
            {
                i--;
                free(env->environ[i]);
            }
            free(env->environ);
            free(env);
            return NULL;
        }
        ft_putstr_fd("[DEBUG] Copied env var: ", 2);
        ft_putstr_fd(env->environ[i], 2);
        ft_putstr_fd("\n", 2);
        i++;
    }
    env->environ[size] = NULL;
    env->last_status = 0;

    return env;
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
