#include "minishell.h"

int execute_external_command(t_ast_node *node, t_fd_info *fd_info, t_env *env)
{
    pid_t pid;
    int status;
    int wstatus;

    pid = fork();
	if (pid == 0)
	{
		if (!handle_command_redirections(node, fd_info))
			exit (1);
		setup_child_signals();
		status = search_and_execute(node->args, env);
		exit(status);
	}
	else if (pid < 0)
		return (perror("minishell: fork"), EXIT_FAILURE);
	else
		status = (ft_decode_wstatus(ft_wait_for_pid(&wstatus, pid)));
    restore_redirections(fd_info);
    return (status);
}

int search_and_execute(char **argv, t_env *env)
{
	int status;
    char *path;
	char *const fullpath = (char[PATH_MAX]){0};

	errno = ENOENT;
	status = try_direct_execution(argv, fullpath);
	if ((status == -1) && (errno == ENOENT || errno == EACCES))
	{
		path = get_env_value("PATH", env);
		status = try_path_execution(path, argv, fullpath);
		free(path);
	}
	if (status == -1)
		perror(path);
	else
		status = try_execute(fullpath, argv, env->environ);
	return (status);
}

int try_direct_execution(char **argv, char *fullpath)
{
	int		errcode;

	errcode = -1;
	if ((*argv[0] == '/') || *argv[0] == '.')
		errcode = access(ft_strncpy(fullpath, argv[0], PATH_MAX), F_OK);
	return (errcode);
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

int try_path_execution(char *path, char **argv, char *const fullpath)
{
    char **paths;
    int i;
    int errcode;

    paths = ft_split(path, ':');
    if (!paths)
		return (EXIT_FAILURE);
    i = -1;
	errcode = -1;
	errno = ENOENT;
    while (((errcode == -1) && (errno == ENOENT || errno == EACCES)) && paths[++i])
	{
		ft_strlcpy(fullpath, paths[i], PATH_MAX);
		ft_strlcat(fullpath, "/", PATH_MAX);
		ft_strlcat(fullpath, argv[0], PATH_MAX);
		errcode = access(fullpath, F_OK);
	}
	free_args_array(paths);
	if ((errcode == -1) && (errno == ENOENT || errno == EACCES))
		errcode = access(argv[0], F_OK);
	else
		errcode = EX_NOTFOUND;
	return (errcode);
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
