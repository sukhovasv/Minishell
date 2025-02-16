#include "minishell.h"

void	get_operator_info(const char *str, t_token_type *type, size_t *len)
{
	*len = 1;
	if (str[0] == '|')
		*type = TOKEN_PIPE;
	else if (str[0] == '<')
	{
		if (str[1] == '<')
		{
			*type = TOKEN_REDIR_HEREDOC;
			*len = 2;
		}
		else
			*type = TOKEN_REDIR_IN;
	}
	else if (str[0] == '>')
	{
		if (str[1] == '>')
		{
			*type = TOKEN_REDIR_APPEND;
			*len = 2;
		}
		else
			*type = TOKEN_REDIR_OUT;
	}
}

static size_t	skip_spaces_and_get_delimiter(const char *str, size_t len,
				size_t *delim_len)
{
	size_t	total_len;
	int		in_quotes;
	char	quote_char;

	total_len = len;
	str += len;
	while (*str && (*str == ' ' || *str == '\t'))
	{
		str++;
		total_len++;
	}
	in_quotes = 0;
	quote_char = 0;
	*delim_len = get_word_length(str, &in_quotes, &quote_char);
	if (*delim_len == 0)
		return (0);
	return (total_len);
}

static size_t	handle_heredoc_delimiter(const char *str, t_token **tokens,
				t_env *env, size_t len)
{
	size_t	total_len;
	size_t	delim_len;
	t_token	*delim_token;
	int		expand_vars;

	total_len = skip_spaces_and_get_delimiter(str, len, &delim_len);
	if (total_len == 0)
		return (0);
	delim_token = create_heredoc_token(str + total_len, delim_len,
			&expand_vars);
	if (!delim_token)
		return (0);
	add_token(tokens, delim_token);
	delim_token->has_quotes = !expand_vars;
	if (!handle_heredoc_content(delim_token, env))
		return (0);
	return (total_len + delim_len);
}

size_t	handle_operator(const char *str, t_token **tokens, t_env *env)
{
	t_token			*token;
	t_token_type	type;
	size_t			len;

	if (str[0] != '|' && str[0] != '<' && str[0] != '>')
		return (0);
	get_operator_info(str, &type, &len);
	token = new_token(type, str, len);
	if (!token)
		return (0);
	add_token(tokens, token);
	if (type == TOKEN_REDIR_HEREDOC)
		return (handle_heredoc_delimiter(str, tokens, env, len));
	return (len);
}
