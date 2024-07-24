/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:15:03 by ssukhova          #+#    #+#             */
/*   Updated: 2024/07/24 16:15:06 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	execute_command(char *cmd, char **envp)
{
	t_path_data	data;
	int			i;

	data.path = find_path(cmd, envp);
	if (data.path == NULL)
	{
		ft_printf("Command not found: %s\n", cmd);
		error_exit("Command not found");
	}
	data.args = ft_split(cmd, ' ');
	if (!data.args || !data.args[0])
	{
		free(data.path);
		error_exit("Parsing command failed");
	}
	execve(data.path, data.args, envp);
	perror("execve");
	free(data.path);
	i = 0;
	while (data.args[i])
		free(data.args[i++]);
	free(data.args);
	error_exit("Command execution failed");
}

void	child_process(char *cmd, int fd_in, int fd_out, char **envp)
{
	if (dup2(fd_in, STDIN_FILENO) == -1)
		error_exit("dup2 input failed");
	if (dup2(fd_out, STDOUT_FILENO) == -1)
		error_exit("dup2 output failed");
	close(fd_in);
	close(fd_out);
	execute_command(cmd, envp);
}

void	handle_child_process(t_pipex *pipex, t_args *args,
		int prev_pipe_fd, int i)
{
	close(pipex->pipe_fd[0]);
	if (dup2(prev_pipe_fd, STDIN_FILENO) == -1)
		error_exit("dup2 input failed");
	if (i != args->end - 1)
	{
		if (dup2(pipex->pipe_fd[1], STDOUT_FILENO) == -1)
			error_exit("dup2 output failed");
	}
	else
	{
		if (dup2(pipex->outfile_fd, STDOUT_FILENO) == -1)
			error_exit("dup2 output failed");
	}
	close(pipex->pipe_fd[1]);
	execute_command(args->argv[i], args->envp);
}

void	handle_parent_process(t_pipex *pipex, int *prev_pipe_fd)
{
	waitpid(pipex->child_pid, NULL, 0);
	close(pipex->pipe_fd[1]);
	if (*prev_pipe_fd != pipex->infile_fd)
		close(*prev_pipe_fd);
	*prev_pipe_fd = pipex->pipe_fd[0];
}
