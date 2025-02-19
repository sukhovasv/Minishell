/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_executor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:23:17 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:23:19 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_single_heredoc(t_redirect *redir, t_fd_info *fd_info)
{
	int	fd;

	if (!redir->token || !redir->token->temp_file)
		return (0);
	fd = open(redir->token->temp_file, O_RDONLY);
	if (fd == -1)
		return (0);
	if (fd_info->saved_stdin == -1)
		fd_info->saved_stdin = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

static int	handle_single_redirection(t_redirect *redir, t_fd_info *fd_info)
{
	if (redir->type == TOKEN_REDIR_HEREDOC)
		return (handle_single_heredoc(redir, fd_info));
	else if (redir->type == TOKEN_REDIR_IN)
		return (handle_redir_input(redir, &fd_info->saved_stdin));
	else if (redir->type == TOKEN_REDIR_OUT)
		return (handle_redir_output(redir, &fd_info->saved_stdout));
	else if (redir->type == TOKEN_REDIR_APPEND)
		return (handle_redir_append(redir, &fd_info->saved_stdout));
	return (1);
}

int	handle_command_redirections(t_ast_node *node, t_fd_info *fd_info)
{
	t_redirect	*redir;

	if (!node->redirects)
		return (1);
	redir = node->redirects;
	while (redir)
	{
		if (!handle_single_redirection(redir, fd_info))
			return (0);
		redir = redir->next;
	}
	return (1);
}
