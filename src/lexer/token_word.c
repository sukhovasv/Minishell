#include "minishell.h"

int	handle_opening_quote(const char *str, size_t *i, int *in_quotes,
			char *quote_char)
{
	if ((str[*i] == '"' || str[*i] == '\'') && !(*in_quotes))
	{
		*quote_char = str[*i];
		*in_quotes = 1;
		(*i)++;
		return (1);
	}
	return (0);
}

int	handle_closing_quote(const char *str, size_t *i, int *in_quotes,
			char *quote_char)
{
	if (*in_quotes && str[*i] == *quote_char)
	{
		*in_quotes = 0;
		*quote_char = 0;
		(*i)++;
		return (1);
	}
	return (0);
}

char	*process_word_content(t_word_parser *parser)
{
	while (parser->i < parser->len)
	{
		if (handle_opening_quote(parser->str, &parser->i,
				&parser->in_quotes, &parser->quote_char))
			continue ;
		if (handle_closing_quote(parser->str, &parser->i,
				&parser->in_quotes, &parser->quote_char))
			continue ;
		if (parser->str[parser->i] == '$' && parser->quote_char != '\'')
		{
			parser->result = handle_env_var(parser);
			if (!parser->result)
				return (NULL);
			parser->i++;
			continue ;
		}
		parser->result[parser->j] = parser->str[parser->i];
		parser->j++;
		parser->i++;
	}
	return (parser->result);
}

char	*create_word_without_quotes(const char *str, size_t len, t_env *env)
{
	t_word_parser	parser;
	char			*result;

	result = malloc(len * 2 + 1);
	if (!result)
		return (NULL);
	parser.str = str;
	parser.result = result;
	parser.i = 0;
	parser.j = 0;
	parser.len = len;
	parser.in_quotes = 0;
	parser.quote_char = 0;
	parser.env = env;
	parser.result = process_word_content(&parser);
	if (parser.result)
		parser.result[parser.j] = '\0';
	return (parser.result);
}

size_t	handle_word(const char *str, t_token **tokens, t_env *env,
			t_parser_state *state)
{
	size_t	len;
	t_token	*token;

	len = get_word_length(str, &state->in_quotes, &state->quote_char);
	if (len == 0)
		return (0);
	if (str[0] == '$')
		token = handle_env_token(str, env);
	else
	{
		token = handle_regular_token(str, len, env);
		if (token && is_last_token_heredoc(*tokens))
			token->has_quotes = has_paired_quotes(str, len);
	}
	if (!token)
		return (0);
	add_token(tokens, token);
	return (len);
}
