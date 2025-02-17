#ifndef PARSER_H
# define PARSER_H

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

int			count_args(t_token *tokens);
char		**create_args_array(int count);
int			copy_token_values(char **args, t_token *tokens, int count);
int			is_redirect_token(t_token_type type);
//t_ast_node	*parse_command(const char *input, t_env *env);
t_ast_node	*build_ast(t_token *tokens);
t_ast_node	*parse_simple_command(t_token **current);
t_ast_node	*parse_pipe_sequence(t_token **current);
void		free_redirections(t_redirect *redirects);
void		free_args(char **args);
void		free_ast_node(t_ast_node *node);
void		free_redirects(t_redirect *redirects);
void		free_args_array(char **args);
t_ast_node	*create_command_node(char **args, t_redirect *redirects);
t_ast_node	*create_pipe_node(t_ast_node *left, t_ast_node *right);
t_ast_node	*create_redirect_node(t_ast_node *cmd, t_redirect *redir);
t_redirect	*create_redirect(t_token_type type, const char *file,
				t_token *token);
int			add_redirect_to_list(t_redirect **redirects, t_redirect *new_redir);
t_ast_node	*handle_command_redirects(t_token **current, char **args,
				t_redirect **redirects);
t_ast_node	*create_pipe_sequence(t_ast_node *left, t_ast_node *right);
int			process_redirect(t_token **current, t_token_type type,
				t_redirect **redirects);
void		cleanup_command(char **args, t_redirect *redirects);

#endif
