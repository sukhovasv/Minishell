/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:24:29 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:24:31 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_current_heredoc(t_heredoc_data *heredocs,
			t_redirect *current, int i)
{
	if (!initialize_single_heredoc(&heredocs[i], current))
	{
		free_initialized_heredocs(heredocs, i);
		return (0);
	}
	return (1);
}

int	process_and_open_last_heredoc(t_heredoc_data *heredocs,
			int count, t_env *env)
{
	int	last_fd;

	if (!handle_signals_and_fork(heredocs, count, env))
	{
		cleanup_heredoc_files(heredocs, count);
		return (-1);
	}
	last_fd = open(heredocs[count - 1].temp_file, O_RDONLY);
	if (last_fd == -1)
	{
		cleanup_heredoc_files(heredocs, count);
		return (-1);
	}
	return (last_fd);
}

int	fork_and_process_heredocs(t_heredoc_data *heredocs, int count, t_env *env)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (write_single_heredoc(&heredocs[i], env) != 0)
			return (-1);
		i++;
	}
	return (0);
}

int	process_heredocs_in_child(t_heredoc_data *heredocs, int count, t_env *env)
{
	setup_heredoc_signals();
	return (fork_and_process_heredocs(heredocs, count, env));
}
