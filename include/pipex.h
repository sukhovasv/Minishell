/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:19:17 by ssukhova          #+#    #+#             */
/*   Updated: 2024/07/24 16:29:23 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include "libft.h"

typedef struct s_path_data
{
	char	**args;
	char	**paths;
	char	*path_var;
	char	*path;
}	t_path_data;

typedef struct s_pipex
{
	int		pipe_fd[2];
	int		infile_fd;
	int		outfile_fd;
	pid_t	child_pid;
}	t_pipex;

typedef struct s_args
{
	char	**argv;
	char	**envp;
	int		start;
	int		end;
}	t_args;

void	error_exit(const char *message);
char	**split_command(char *cmd);
char	*get_path_variable(char **envp);
char	**split_path_variable(char *path_var);
char	*find_executable(char *cmd, char **paths);
void	free_path_data(t_path_data *data);
char	*find_path(char *cmd, char **envp);
void	execute_command(char *cmd, char **envp);
void	child_process(char *cmd, int fd_in, int fd_out, char **envp);
void	process_normal_mode(char **argv, char **envp, int cmd_count);
void	validate_args(int argc, char **argv);
int		open_file(char *file, int flags, mode_t mode);
void	create_pipe(int pipe_fd[2]);
pid_t	create_child(void);
void	process_here_doc(char **argv, int argc, char **envp);
void	parent_process(t_pipex *pipex, char **argv, char **envp, int cmd_count);
void	process_commands(t_pipex *pipex, t_args *args);
void	setup_pipes_and_fork(t_pipex *pipex, t_args *args,
			int *prev_pipe_fd, int i);
void	handle_child_process(t_pipex *pipex, t_args *args,
			int prev_pipe_fd, int i);
void	handle_parent_process(t_pipex *pipex, int *prev_pipe_fd);
void	handle_here_doc(char *limiter);
char	*trim_str(char *str);

#endif
