#include "minishell.h"

static void free_and_error(char **dirs, char *path, char *cmd)
{
    int i;

    i = 0;
    while (dirs[i])
        free(dirs[i++]);
    free(dirs);
    free(path);
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(cmd, 2);
    ft_putendl_fd(": command not found", 2);
    exit(127);
}

static void try_paths(char **dirs, char **argv, char **envp)
{
    int     i;
    char    full_path[PATH_MAX];

    i = 0;
    while (dirs[i])
    {
        ft_strlcpy(full_path, dirs[i], PATH_MAX);
        ft_strlcat(full_path, "/", PATH_MAX);
        ft_strlcat(full_path, argv[0], PATH_MAX);
        if (try_execute(full_path, argv, envp) == 0)
            i++;
    }
}

void try_path_execution(char *path, char **argv, char **envp)
{
    char    **dirs;

    dirs = ft_split(path, ':');
    if (!dirs)
    {
        free(path);
        ft_putstr_fd("minishell: memory allocation error\n", 2);
        exit(1);
    }
    try_paths(dirs, argv, envp);
    free_and_error(dirs, path, argv[0]);
}

char *find_command_in_path(char *cmd)
{
    char *path_env;
    char **paths;
    char *cmd_path;
    int i;

    path_env = getenv("PATH");
    if (!path_env)
        return (NULL);

    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);

    i = 0;
    while (paths[i])
    {
        cmd_path = ft_strjoin(paths[i], "/");
        cmd_path = ft_strjoin(cmd_path, cmd);
        if (access(cmd_path, X_OK) == 0)
        {
            // Освобождаем память для `paths`
            int j = 0;
            while (paths[j])
            {
                free(paths[j]);
                j++;
            }
            free(paths);
            return (cmd_path);
        }
        free(cmd_path);
        i++;
    }

    // Освобождаем `paths`, если команда не найдена
    int j = 0;
    while (paths[j])
    {
        free(paths[j]);
        j++;
    }
    free(paths);
    return (NULL);
}

/*static void free_and_error(char **dirs, char *path, char *cmd)
{
    int i;

    i = 0;
    while (dirs[i])
        free(dirs[i++]);
    free(dirs);
    free(path);
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(cmd, 2);
    ft_putendl_fd(": command not found", 2);
    exit(127);
}*/
