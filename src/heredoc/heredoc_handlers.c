/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:24:08 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:24:10 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	write_single_heredoc(t_heredoc_data *data, t_env *env)
{
	int	fd;

	fd = open_temp_file(data->temp_file);
	if (fd == -1)
		return (-1);
	if (read_and_write_lines(fd, data, env) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_signals_and_fork(t_heredoc_data *heredocs, int count, t_env *env)
{
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		setup_heredoc_signals(); //todo?
		if (process_heredocs_in_child(heredocs, count, env) != 0)
			builtin_exit_wrapper(env, 1);
		builtin_exit_wrapper(env, 0);
	}
	ignore_signals();
	waitpid(pid, &status, 0);
	setup_signals(env);
	if (g_signal_received == SIGINT)
	{
		g_signal_received = 0;
		return (0);
	}
	return ((WIFEXITED(status) && WEXITSTATUS(status) == 0));
}

/*int	is_heredoc_without_command(t_redirect *redir, t_ast_node *ast)
{
	if (!redir)
		return (1);
	if (ast && ast->args && ast->args[0] && ast->args[0][0] != '\0')
		return (0);
	return (1);
}*/

int is_heredoc_without_command(t_redirect *redir, t_ast_node *ast)
{
    ft_putstr_fd("\nDebug: Checking heredoc command\n", 2);
    
    if (!redir)
    {
        ft_putstr_fd("Debug: No redirects\n", 2);
        return (1);
    }
    
    ft_putstr_fd("Debug: AST check: ", 2);
    if (ast)
        ft_putstr_fd("ast exists, ", 2);
    if (ast && ast->args)
        ft_putstr_fd("args exist, ", 2);
    if (ast && ast->args && ast->args[0])
        ft_putstr_fd("args[0] exists, ", 2);
    if (ast && ast->args && ast->args[0] && ast->args[0][0] != '\0')
        ft_putstr_fd("args[0] not empty", 2);
    ft_putstr_fd("\n", 2);
    
    if (ast && ast->args && ast->args[0] && ast->args[0][0] != '\0')
        return (0);
    return (1);
}

int	handle_redir_heredoc(t_redirect *redir, t_fd_info *fd_info,
			t_env *env, t_ast_node *ast)
{
	t_heredoc_data	*heredocs;
	int				count;
	int				last_fd;

	count = prepare_heredoc_data(redir, &heredocs);
	if (count == 0)
		return (0);
	last_fd = process_and_open_last_heredoc(heredocs, count, env);
	if (last_fd == -1)
		return (0);
	if (is_heredoc_without_command(redir, ast))
	{
		close(last_fd);
		return (0);
	}
	if (fd_info->saved_stdin == -1)
		fd_info->saved_stdin = dup(STDIN_FILENO);
	if (dup2(last_fd, STDIN_FILENO) == -1)
	{
		close(last_fd);
		return (0);
	}
	close(last_fd);
	return (1);
}
