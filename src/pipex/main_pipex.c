/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:15:22 by ssukhova          #+#    #+#             */
/*   Updated: 2024/07/24 16:16:09 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	t_args	args;

	validate_args(argc, argv);
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		process_here_doc(argv, argc, envp);
	else
	{
		pipex.infile_fd = open_file(argv[1], O_RDONLY, 0);
		pipex.outfile_fd = open_file(argv[argc - 1],
				O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR
				| S_IWUSR | S_IRGRP | S_IROTH);
		args.argv = argv;
		args.envp = envp;
		args.start = 2;
		args.end = argc - 1;
		process_commands(&pipex, &args);
	}
	close(pipex.infile_fd);
	close(pipex.outfile_fd);
	return (EXIT_SUCCESS);
}
