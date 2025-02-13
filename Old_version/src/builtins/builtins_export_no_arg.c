#include "minishell.h"

static void swap_strings(char **a, char **b)
{
    char *temp = *a;
    *a = *b;
    *b = temp;
}

static void sort_env_array(char **env, int count)
{
    int i = 0;
    while (i < count - 1)
    {
        int j = i + 1;
        while (j < count)
        {
            // Используем ft_strncmp, чтобы сравнить строки
            if (ft_strncmp(env[i], env[j], ft_strlen(env[i]) > 0))
                swap_strings(&env[i], &env[j]);
            j++;
        }
        i++;
    }
}

static void print_single_env_var(char *env_var)
{
    char *equal_sign = ft_strchr(env_var, '=');
    if (equal_sign)
    {
        *equal_sign = '\0';
        printf("declare -x %s=\"%s\"\n", env_var, equal_sign + 1);
        *equal_sign = '='; // Восстанавливаем строку
    }
    else
    {
        printf("declare -x %s\n", env_var);
    }
}

void print_sorted_env(t_env *env)
{
    int count = count_env_vars(env->environ);
    char **sorted_env = allocate_new_environ(env->environ, 0);
    if (!sorted_env)
        return;

    sort_env_array(sorted_env, count);

    int i = 0;
    while (sorted_env[i])
    {
        print_single_env_var(sorted_env[i]);
        i++;
    }

    free_args_array(sorted_env);
}
