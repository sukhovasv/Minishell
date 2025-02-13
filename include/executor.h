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
int	execute_command(char *input, t_env *env);
int execute_ast_node(t_ast_node *node, t_env *env, t_fd_info *fd_info);
int execute_command_node(t_ast_node *node, t_env *env, t_fd_info *fd_info);
int execute_builtin_command(t_ast_node *node, t_env *env, t_fd_info *fd_info);
int execute_external_command(t_ast_node *node, t_fd_info *fd_info);
int	execute_non_builtin(char **argv, t_env *env);
void	search_and_execute(char **argv, char **envp);
void	try_direct_execution(char **argv, char **envp);
int	try_execute(char *cmd, char **argv, char **envp);
char	*get_path_env(char **argv);
char *find_command_in_path(char *cmd);
void try_path_execution(char *path, char **argv, char **envp);
int execute_pipe_node(t_ast_node *node, t_env *env, t_fd_info *fd_info);
void execute_command_in_pipe(t_ast_node *cmd, t_env *env, t_fd_info *fd_info);
int handle_pipe_redirect_out(t_redirect *redir);
int execute_ast_redirect_node(t_ast_node *node, t_env *env, t_fd_info *fd_info);
int handle_command_redirections(t_ast_node *node, t_fd_info *fd_info);
int handle_single_heredoc(t_redirect *redir, t_fd_info *fd_info);
void free_and_error(char **dirs, char *path, char *cmd);

#endif
