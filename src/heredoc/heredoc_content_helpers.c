/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_content_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:24:01 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:24:03 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_line_to_heredoc(char *line, int fd)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

char	*handle_variable_expansion(char *line, t_env *env)
{
	char	*expanded;

	if (ft_strchr(line, '$'))
	{
		expanded = expand_env_variables(line, env);
		if (expanded)
		{
			free(line);
			return (expanded);
		}
	}
	return (line);
}

int	check_heredoc_line(char **line, t_token *token,
						t_env *env, int expand_vars)
{
	if (!line)
	{
		ft_putstr_fd(WRONG_DELIMITER, STDERR_FILENO);
		return (0);
	}
	if (g_signal_received == SIGINT)
		return (-1);
	if (is_end_of_heredoc(*line, token->value))
	{
		free(*line);
		return (0);
	}
	if (expand_vars)
		*line = handle_variable_expansion(*line, env);
	return (1);
}

int	cleanup_heredoc(char *line, t_token *token, int fd)
{
	free(line);
	close(fd);
	unlink(token->temp_file);
	free(token->temp_file);
	token->temp_file = NULL;
	return (0);
}

int	success_heredoc(int fd, t_env *env)
{
	close(fd);
	setup_signals(env);
	return (1);
}
