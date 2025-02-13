#include "minishell.h"

int is_str_digit(const char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

int builtin_echo(char **argv)
{
    int i;
    int n_flag;

    n_flag = 0;
    i = 1;

    if (argv[1] && ft_strncmp(argv[1], "-n", 2) == 0)
    {
        n_flag = 1;
        i++;
    }

    while (argv[i])
    {
        ft_putstr_fd(argv[i], STDOUT_FILENO);
        if (argv[i + 1])
            ft_putchar_fd(' ', STDOUT_FILENO);
        i++;
    }

    if (!n_flag)
        ft_putchar_fd('\n', STDOUT_FILENO);
    
    return (0);
}

int builtin_cd(char **argv)
{
    extern char **environ;
    t_env *env;
    char path[PATH_MAX];
    char *home;

    env = init_env(environ);
    if (!argv[1])
    {
        home = get_env_value("HOME", env);  // Функция поиска в env->environ
        if (!home)
        {
            ft_putendl_fd("cd: HOME environment variable not set", 2);
            return (1);
        }
        if (chdir(home) != 0)
        {
            perror("cd");
            return (1);
        }
    }
    else if (chdir(argv[1]) != 0)
    {
        perror("cd");
        return (1);
    }
    
    if (getcwd(path, PATH_MAX))
        update_existing_variable(env, "PWD", path);  // Функция обновления в env->environ
    return (0);
}

int builtin_pwd(void)
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)))
    {
        ft_putendl_fd(cwd, STDOUT_FILENO);
        return (0);
    }
    
    perror("pwd");
    return (1);
}

int builtin_exit(char **argv)
{
    int status;
    
    status = 0;
    if (argv[1])
    {
        if (!is_str_digit(argv[1]))
        {
            ft_putstr_fd("exit: ", 2);
            ft_putstr_fd(argv[1], 2);
            ft_putendl_fd(": numeric argument required", 2);
            exit(255);
        }
        status = ft_atoi(argv[1]);
        if (argv[2])
        {
            ft_putendl_fd("exit: too many arguments", 2);
            return (1);
        }
    }
    exit(status & 0xFF);
}
