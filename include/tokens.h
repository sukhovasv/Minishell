#ifndef MINISHELL_TOKENS_H
# define MINISHELL_TOKENS_H

typedef enum	e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
}				t_token_type;

typedef struct	s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
}				t_token;

t_token	*init_token(char *value, t_token_type type);
void	append_token(t_token **list, t_token *new_token);
void	append_word_token(char *start, char *end, t_token **token_list);
char	*tokenise_special_chars(char *str, t_token **tokens);
char	*tokenise_phrase(char *str, t_token **tokens);
t_token	*tokenise(char *str);


#endif //MINISHELL_TOKENS_H
