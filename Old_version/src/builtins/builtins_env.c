#include "minishell.h"

static int should_print_var(const char *var)
{
    return (ft_strchr(var, '=') != NULL);
}

int builtin_env(char **environ)
{
    int i;

    if (!environ)
        return (1);
    i = 0;
    while (environ[i])
    {
        if (should_print_var(environ[i]))
            ft_putendl_fd(environ[i], STDOUT_FILENO);
        i++;
    }
    return (0);
}