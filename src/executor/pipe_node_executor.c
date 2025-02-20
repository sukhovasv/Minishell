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

void	handle_first_child(int *pipefd, t_ast_node *node,
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

void	handle_second_child(int *pipefd, t_ast_node *node,
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

int	handle_pipe_error(int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
	return (1);
}

int	handle_fork_error(int *pipefd, pid_t pid1)
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
