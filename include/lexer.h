#ifndef LEXER_H
# define LEXER_H

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

int		is_delimiter(char c);
void	get_operator_info(const char *str, t_token_type *type, size_t *len);
void	add_token(t_token **head, t_token *new_token);
void	free_tokens(t_token *tokens);
size_t handle_operator(const char *str, t_token **tokens, t_env *env);
size_t	handle_word(const char *str, t_token **tokens, t_env *env, t_parser_state *state);
size_t	process_token(const char *input, t_token **tokens, t_env *env, t_parser_state *state);
size_t	get_token_length(const char *str, int *in_quotes, char *quote_char);
t_token	*new_token(t_token_type type, const char *value, size_t len);
t_token	*tokenize(const char *input, t_env *env);
char *find_env_value(const char *var_name, size_t len, t_env *env);
t_token *create_heredoc_token(const char *str, size_t len, int *expand_vars);
char	*process_token_value(const char *value, size_t len, int *has_quotes);
void	update_index_after_env_var(t_word_parser *parser);
void	init_tokenizer(t_token **tokens, size_t *i, t_parser_state *state);
char	*create_word_without_quotes(const char *str, size_t len, t_env *env);
char	*reallocate_result(char *result, char *env_val, size_t j,
			size_t env_len);
void	copy_env_value(char *result, const char *env_val, size_t *j);
size_t	get_word_length(const char *str, int *in_quotes, char *quote_char);
char	*handle_env_var(t_word_parser *parser);
t_token	*handle_env_token(const char *str, t_env *env);
t_token	*handle_regular_token(const char *str, size_t len, t_env *env);
int	is_last_token_heredoc(t_token *tokens);
int	has_paired_quotes(const char *str, size_t len);

#endif
