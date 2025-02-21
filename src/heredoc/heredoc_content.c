/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_content.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:24:01 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:24:03 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc_content(t_token *token, t_env *env)
{
	char	*line;
	int		fd;
	int		expand_vars;
	int		status;

	fd = init_heredoc_file(token);
	if (fd == -1)
		return (0);
	expand_vars = !token->has_quotes;
	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("warning: here-documentdelimited by end-of-file\n", STDERR_FILENO);
			break ;
		}
		status = check_heredoc_line(&line, token, env, expand_vars);
		if (status == -1)
			return (cleanup_heredoc(line, token, fd));
		if (status == 0)
			break ;
		write_line_to_heredoc(line, fd);
		free(line);
	}
	return (success_heredoc(fd, env));
}
