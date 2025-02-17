#include "minishell.h"

int execute_external_command(t_ast_node *node, t_fd_info *fd_info, t_env *env)
{
    pid_t pid;
    int status;

    if (!handle_command_redirections(node, fd_info))
        return (1);

    pid = fork();
    if (pid == -1)
    {
        perror("minishell: fork");
        return (1);
    }

    if (pid == 0)
    {
        setup_child_signals();
        search_and_execute(node->args, env->environ);
        exit(127);
    }

    waitpid(pid, &status, 0);
    restore_redirections(fd_info);
    
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    return (1);
}

void search_and_execute(char **argv, char **envp)
{
    char *path;

    try_direct_execution(argv, envp);
    path = get_path_env(argv);
    try_path_execution(path, argv, envp);
}

void try_direct_execution(char **argv, char **envp)
{
    if (ft_strchr(argv[0], '/'))
    {
        try_execute(argv[0], argv, envp);
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(argv[0], 2);
        ft_putendl_fd(": No such file or directory", 2);
        exit(127);
    }
}

char *get_path_env(char **argv)
{
    char *path_env;
    char *path;

    path_env = getenv("PATH");
    if (!path_env)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(argv[0], 2);
        ft_putendl_fd(": No such file or directory", 2);
        exit(127);
    }
    path = ft_strdup(path_env);
    if (!path)
        exit(1);
    return (path);
}

void try_path_execution(char *path, char **argv, char **envp)
{
    char **paths;
    char *full_cmd;
    char *temp;
    int i;

    paths = ft_split(path, ':');
    if (!paths)
        exit(1);
    
    i = 0;
    while (paths[i])
    {
        temp = ft_strjoin(paths[i], "/");
        if (!temp)
            exit(1);
        full_cmd = ft_strjoin(temp, argv[0]);
        free(temp);
        if (!full_cmd)
            exit(1);
        if (try_execute(full_cmd, argv, envp))
            free(full_cmd);
        i++;
    }
    free_args_array(paths);
    free(path);
    
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(argv[0], 2);
    ft_putendl_fd(": command not found", 2);
    exit(127);
}

int try_execute(char *cmd, char **argv, char **envp)
{
    if (access(cmd, F_OK) == -1)
        return (0);
    if (access(cmd, X_OK) == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd, 2);
        ft_putendl_fd(": Permission denied", 2);
        exit(126);
    }
    execve(cmd, argv, envp);
    return (0);
}
