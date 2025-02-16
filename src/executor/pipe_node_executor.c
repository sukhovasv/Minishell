#include "minishell.h"

static void	handle_first_child(int *pipefd, t_ast_node *node,
			t_env *env, t_fd_info *fd_info)
{
	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		exit(1);
	close(pipefd[1]);
	execute_command_in_pipe(node->left, env, fd_info);
	exit(1);
}

static void	handle_second_child(int *pipefd, t_ast_node *node,
			t_env *env, t_fd_info *fd_info)
{
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		exit(1);
	close(pipefd[0]);
	if (node->right->type == AST_PIPE)
		execute_pipe_node(node->right, env, fd_info);
	else
		execute_command_in_pipe(node->right, env, fd_info);
	exit(1);
}

static int	handle_pipe_error(int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
	return (1);
}

static int	handle_fork_error(int *pipefd, pid_t pid1)
{
	close(pipefd[0]);
	close(pipefd[1]);
	if (pid1 > 0)
	{
		kill(pid1, SIGTERM);
		waitpid(pid1, NULL, 0);
	}
	return (1);
}

int	execute_pipe_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (pipe(pipefd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 < 0)
		return (handle_pipe_error(pipefd));
	if (pid1 == 0)
		handle_first_child(pipefd, node, env, fd_info);
	pid2 = fork();
	if (pid2 < 0)
		return (handle_fork_error(pipefd, pid1));
	if (pid2 == 0)
		handle_second_child(pipefd, node, env, fd_info);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	return (WEXITSTATUS(status));
}
