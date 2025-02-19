/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_processor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:29:58 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:29:59 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_non_heredoc_redirections(t_redirect *redirects, t_fd_info *fd_info)
{
	t_redirect	*current;
	int			status;

	current = redirects;
	while (current)
	{
		if (current->type != TOKEN_REDIR_HEREDOC)
		{
			if (current->type == TOKEN_REDIR_IN)
				status = handle_redir_input(current, &fd_info->saved_stdin);
			else if (current->type == TOKEN_REDIR_OUT)
				status = handle_redir_output(current, &fd_info->saved_stdout);
			else if (current->type == TOKEN_REDIR_APPEND)
				status = handle_redir_append(current, &fd_info->saved_stdout);
			if (!status)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	handle_redirections(t_redirect *redirects, t_fd_info *fd_info,
			t_env *env, t_ast_node *ast)
{
	fd_info->saved_stdout = -1;
	fd_info->saved_stdin = -1;
	if (!handle_heredoc_redirections(redirects, fd_info, env, ast))
		return (0);
	if (!handle_non_heredoc_redirections(redirects, fd_info))
		return (0);
	return (1);
}
