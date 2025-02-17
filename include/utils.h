#ifndef UTILS_H
# define UTILS_H

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

void	restore_fds(int saved_stdout, int saved_stdin);
int		handle_command_error(t_ast_node *cmd, int saved_stdout,
			int saved_stdin);
char	*extract_var_name(const char *input, int *i);
char	*process_dollar(const char *input, int *i, t_env *env);
char	*handle_env_vars(const char *input, int *i, t_env *env);
char	*parse_quotes(const char *input, t_env *env);
void	init_quote_state(int *i, int *j, int *in_single_quotes,
			int *in_double_quotes);
char	*allocate_result_buffer(const char *input);
void	handle_quote(char c, int *in_single_quotes, int *in_double_quotes);
int		handle_env_variable(const char *input, t_str_processor *proc,
			t_env *env, int in_single_quotes);
int		handle_redir_input(t_redirect *redir, int *saved_stdin);
int		handle_redir_output(t_redirect *redir, int *saved_stdout);
int		handle_redir_append(t_redirect *redir, int *saved_stdout);
void	restore_redirections(t_fd_info *fd_info);
int		handle_redirections(t_redirect *redirects, t_fd_info *fd_info,
			t_env *env, t_ast_node *ast);
int		handle_heredoc_redirections(t_redirect *redirects, t_fd_info *fd_info,
			t_env *env, t_ast_node *ast);
int		handle_non_heredoc_redirections(t_redirect *redirects,
			t_fd_info *fd_info);
void	free_heredoc_data(t_heredoc_data *heredoc);
void	free_all(t_minishell_data *data);

#endif
