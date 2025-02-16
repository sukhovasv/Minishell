#ifndef HEREDOC_H
# define HEREDOC_H

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

void cleanup_heredoc_files(t_heredoc_data *heredocs, int count);
int free_initialized_heredocs(t_heredoc_data *heredocs, int count);
int write_single_heredoc(t_heredoc_data *data, t_env *env);
int handle_signals_and_fork(t_heredoc_data *heredocs, int count, t_env *env);
int is_heredoc_without_command(t_redirect *redir, t_ast_node *ast);
int handle_redir_heredoc(t_redirect *redir, t_fd_info *fd_info, t_env *env, t_ast_node *ast);
int allocate_heredoc_data(int count, t_heredoc_data **heredocs);
int initialize_single_heredoc(t_heredoc_data *heredoc, t_redirect *current);
int initialize_heredocs(t_redirect *redir, t_heredoc_data *heredocs);
int count_heredocs(t_redirect *redir);
int prepare_heredoc_data(t_redirect *redir, t_heredoc_data **heredocs);
int process_current_heredoc(t_heredoc_data *heredocs, t_redirect *current, int i);
int process_and_open_last_heredoc(t_heredoc_data *heredocs, int count, t_env *env);
int fork_and_process_heredocs(t_heredoc_data *heredocs, int count, t_env *env);
int process_heredocs_in_child(t_heredoc_data *heredocs, int count, t_env *env);
int	handle_heredoc_content(t_token *token, t_env *env);
char *create_temp_file(void);
int open_temp_file(const char *temp_file);
int is_end_of_heredoc(const char *line, const char *delimiter);
int read_and_write_lines(int fd, t_heredoc_data *data, t_env *env);
void setup_input_redirection(int last_fd, t_fd_info *fd_info);
int has_any_quotes(const char *str);
int should_expand_heredoc_vars(const char *delimiter);
char *clean_delimiter(const char *str);
int	init_heredoc_file(t_token *token);

#endif
