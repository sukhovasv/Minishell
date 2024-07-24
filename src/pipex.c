/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:15:30 by ssukhova          #+#    #+#             */
/*   Updated: 2024/07/24 16:15:32 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	process_commands(t_pipex *pipex, t_args *args)
{
	int	i;
	int	prev_pipe_fd;

	i = args->start;
	prev_pipe_fd = pipex->infile_fd;
	while (i < args->end)
	{
		setup_pipes_and_fork(pipex, args, &prev_pipe_fd, i);
		i++;
	}
}

void	setup_pipes_and_fork(t_pipex *pipex, t_args *args,
		int *prev_pipe_fd, int i)
{
	if (pipe(pipex->pipe_fd) == -1)
		error_exit("Pipe creation failed");
	pipex->child_pid = fork();
	if (pipex->child_pid == -1)
		error_exit("Fork failed");
	if (pipex->child_pid == 0)
		handle_child_process(pipex, args, *prev_pipe_fd, i);
	else
		handle_parent_process(pipex, prev_pipe_fd);
}
