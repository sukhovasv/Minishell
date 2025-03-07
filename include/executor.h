/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                  		:+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:19:07 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:19:11 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <errno.h>
# include <limits.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

t_ast_node	*prepare_command(char *input, t_env *env);
int			execute_command(char *input, t_env *env);
int			execute_ast_node(t_ast_node *node, t_env *env, t_fd_info *fd_info);
int			execute_command_node(t_ast_node *node, t_env *env,
				t_fd_info *fd_info);
int			execute_builtin_command(t_ast_node *node, t_env *env,
				t_fd_info *fd_info);
int			execute_non_builtin(char **argv, t_env *env);
int			search_and_execute(char **argv, t_env *env);
char		*find_command_in_path(char *cmd);
int			try_path_execution(char *path, char **argv, char *const fullpath);
int			execute_pipe_node(t_ast_node *node, t_env *env, t_fd_info *fd_info);
void		execute_command_in_pipe(t_ast_node *cmd, t_env *env,
				t_fd_info *fd_info);
int			handle_pipe_redirect_out(t_redirect *redir);
int			execute_ast_redirect_node(t_ast_node *node, t_env *env,
				t_fd_info *fd_info);
int			handle_command_redirections(t_ast_node *node, t_fd_info *fd_info);
int			handle_single_heredoc(t_redirect *redir, t_fd_info *fd_info);
void		free_and_error(char **dirs, char *path, char *cmd);
int			execute_external_command(t_ast_node *node, t_fd_info *fd_info,
				t_env *env);
int			search_and_execute(char **argv, t_env *env);
int			try_direct_execution(char **argv, char *fullpath);
int			try_execute(char *cmd, char **argv, t_env *env);
void		handle_first_child(int *pipefd, t_ast_node *node,
				t_env *env, t_fd_info *fd_info);
void		handle_second_child(int *pipefd, t_ast_node *node,
				t_env *env, t_fd_info *fd_info);
int			handle_pipe_error(int *pipefd);
int			handle_fork_error(int *pipefd, pid_t pid1);

#endif
