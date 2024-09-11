/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:15:14 by ssukhova          #+#    #+#             */
/*   Updated: 2024/07/24 16:15:16 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	handle_here_doc(char *limiter)
{
	char	*line;
	int		fd;

	fd = open("/tmp/.here_doc", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR
				| S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1)
		error_exit("Opening here_doc file failed");
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			error_exit("Reading from stdin failed");
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
	close(fd);
}

void	process_here_doc(char **argv, int argc, char **envp)
{
	t_pipex	pipex;
	t_args	args;

	handle_here_doc(argv[2]);
	pipex.infile_fd = open("/tmp/.here_doc", O_RDONLY);
	if (pipex.infile_fd == -1)
		error_exit("Opening here_doc temp file failed");
	pipex.outfile_fd = open_file(argv[argc - 1], O_WRONLY | O_CREAT
			| O_APPEND, S_IRUSR
				| S_IWUSR | S_IRGRP | S_IROTH);
	args.argv = argv;
	args.envp = envp;
	args.start = 3;
	args.end = argc - 1;
	process_commands(&pipex, &args);
	unlink("/tmp/.here_doc");
}
