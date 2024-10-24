#include "minishell.h"

/*
count_env_vars считает переменные окружения в массиве строк env

allocate_new_environ - выделяет память для массива указателей на строки
переменных окружения ( +2 это для нового элемента + '\0')

copy_existing_vars - для того чтобы изменить массив переменных окружения,
мы копируем старый массив в новый массив.

create_new_var - создает новую переменную окружения в массиве

add_new_variable - добавляет новую переменную окружения в структуру t_env
*/

int count_env_vars(char **env)
{
    int env_size;

    env_size = 0;
    while (env[env_size])
        env_size++;
    return (env_size);
}

char **allocate_new_environ(int env_size)
{
    return (malloc((env_size + 2) * sizeof(char *)));
}

void copy_existing_vars(char **new_environ, char **old_environ, int env_size)
{
    int i;
    
    i = 0;
    while (i < env_size)
    {
        new_environ[i] = old_environ[i];
        i++;
    }
}

void create_new_var(char **new_environ, int env_size, const char *key, const char *value)
{
    size_t new_var_size;

    new_var_size = ft_strlen(key) + ft_strlen(value) + 2; // +1 для '=' и +1 для '\0'
    new_environ[env_size] = malloc(new_var_size);
    if (new_environ[env_size])
    {
        ft_strlcpy(new_environ[env_size], key, new_var_size); //копируем строку key в выделенную память
        ft_strlcpy(new_environ[env_size] + ft_strlen(key), "=", 2); // +1 для '=' и +1 для '\0' (копируем равно в выделенную память + key)
        ft_strlcpy(new_environ[env_size] + ft_strlen(key) + 1, value, ft_strlen(value) + 1); // копируем значение после "="
        new_environ[env_size][new_var_size - 1] = '\0';
    }
}

void add_new_variable(t_env *env_struct, const char *key, const char *value)
{
    int env_size;
    char **new_environ ;

    env_size = count_env_vars(env_struct->env); //определяем размер массива переменных окружения
    new_environ = allocate_new_environ(env_size); //выделяем помять под этот размер
    if (!new_environ)
        return;
    copy_existing_vars(new_environ, env_struct->env, env_size); //копируем существующие переменные в эту память
    create_new_var(new_environ, env_size, key, value); //создается новая переменная и добавляется в массив new_environ
    new_environ[env_size + 1] = NULL; //завершаем массив строк
    env_struct->env = new_environ; //заменяем старое окружение на новое, переназначив указатель на новый массив
}