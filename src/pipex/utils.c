/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:15:39 by ssukhova          #+#    #+#             */
/*   Updated: 2024/07/24 16:15:40 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_exit(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

void	validate_args(int argc, char **argv)
{
	if (argc < 5)
	{
		ft_printf("Usage: %s file1 cmd1 cmd2 ... cmdn file2\n", argv[0]);
		exit(EXIT_FAILURE);
	}
}

int	open_file(char *file, int flags, mode_t mode)
{
	int	fd;

	fd = open(file, flags, mode);
	if (fd == -1)
		error_exit("Opening file failed");
	return (fd);
}

void	create_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
		error_exit("Pipe creation failed");
}

pid_t	create_child(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error_exit("Fork failed");
	return (pid);
}
