/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:24:55 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:24:57 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_end_of_heredoc(const char *line, const char *delimiter)
{
	return (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
		&& ft_strlen(line) == ft_strlen(delimiter));
}

static int	process_heredoc_line(char *line, t_heredoc_data *data,
			t_env *env, int fd)
{
	char	*expanded;

	if (!line || g_signal_received == SIGINT)
	{
		free(line);
		return (-1);
	}
	if (is_end_of_heredoc(line, data->delimiter))
	{
		free(line);
		return (0);
	}
	if (!(data->token && data->token->has_quotes) && ft_strchr(line, '$'))
	{
		expanded = expand_env_variables(line, env);
		free(line);
		line = expanded;
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
	return (1);
}

int	read_and_write_lines(int fd, t_heredoc_data *data, t_env *env)
{
	char	*line;
	int		status;

	while (1)
	{
		line = readline("> ");
		status = process_heredoc_line(line, data, env, fd);
		if (status == -1)
			return (-1);
		if (status == 0)
			break ;
	}
	return (0);
}
