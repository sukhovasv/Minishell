/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_node_executor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:23:08 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:23:10 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_first_child(int *pipefd, t_ast_node *node,
			t_env *env, t_fd_info *fd_info)
{
	reset_sighandlers(env);
	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		builtin_exit_wrapper(env, 1);
	close(pipefd[1]);
	execute_command_in_pipe(node->left, env, fd_info);
	builtin_exit_wrapper(env, 1);
}

static void	handle_second_child(int *pipefd, t_ast_node *node,
			t_env *env, t_fd_info *fd_info)
{
	reset_sighandlers(env);
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		builtin_exit_wrapper(env, 1);
	close(pipefd[0]);
	if (node->right->type == AST_PIPE)
		execute_pipe_node(node->right, env, fd_info);
	else
		execute_command_in_pipe(node->right, env, fd_info);
	builtin_exit_wrapper(env, 1);
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

int	ft_wait_for_pid(int *wstatus, pid_t pid)
{
	waitpid(pid, wstatus, WUNTRACED);
	while (!WIFEXITED((*wstatus)) && !WIFSIGNALED((*wstatus)))
		waitpid(pid, wstatus, WUNTRACED);
	return (*wstatus);
}

int	ft_decode_wstatus(int wstatus)
{
	int	status;

	status = (EXIT_FAILURE);
	if (WIFSIGNALED (wstatus))
	{
		if (WCOREDUMP(wstatus))
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		else if (WTERMSIG (wstatus) == SIGKILL)
			ft_putstr_fd ("Killed\n", STDERR_FILENO);
		status = (128 + WTERMSIG (wstatus));
	}
	else if (WIFSTOPPED(wstatus))
		status = (128 + WSTOPSIG(wstatus));
	else if (WIFEXITED(wstatus))
		status = (WEXITSTATUS (wstatus));
	return (status);
}

static void	cleanup_heredocs_for_node(t_ast_node *node)
{
	t_heredoc_data	*heredocs;
	int				count;

	if (node && node->redirects)
	{
		count = prepare_heredoc_data(node->redirects, &heredocs);
		if (count > 0)
			cleanup_heredoc_files(heredocs, count);
	}
}

int	execute_pipe_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
	int				pipefd[2];
	pid_t			pid1;
	pid_t			pid2;
	int				status;

	if (pipe(pipefd) == -1)
		return (1);
	signal(SIGINT, SIG_IGN);
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
	cleanup_heredocs_for_node(node->left);
	return (ft_decode_wstatus(status));
}
