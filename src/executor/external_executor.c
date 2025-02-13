#include "minishell.h"

int execute_external_command(t_ast_node *node, t_fd_info *fd_info)
{
    pid_t pid;
    int status;

    if (!handle_command_redirections(node, fd_info))
        return 1;

    pid = fork();
    if (pid == -1)
        return 1;
    if (pid == 0)
    {
        setup_child_signals();
        execvp(node->args[0], node->args);
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(node->args[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        exit(127);
    }

    waitpid(pid, &status, 0);
    restore_redirections(fd_info);
    return WEXITSTATUS(status);
}

int	execute_non_builtin(char **argv, t_env *env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
		search_and_execute(argv, env->environ);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

void	search_and_execute(char **argv, char **envp)
{
	char	*path;

	try_direct_execution(argv, envp);
	path = get_path_env(argv);
	try_path_execution(path, argv, envp);
}

void	try_direct_execution(char **argv, char **envp)
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

int	try_execute(char *cmd, char **argv, char **envp)
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
