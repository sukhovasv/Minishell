#include "minishell.h"

size_t	process_token(const char *input, t_token **tokens, t_env *env,
			t_parser_state *state)
{
	size_t	len;

	if (input[0] == '|' || input[0] == '<' || input[0] == '>')
		len = handle_operator(input, tokens, env);
	else
		len = handle_word(input, tokens, env, state);
	if (!len)
	{
		free_tokens(*tokens);
		*tokens = NULL;
		return (0);
	}
	return (len);
}

t_token	*tokenize(const char *input, t_env *env)
{
	t_token			*tokens;
	size_t			i;
	size_t			len;
	t_parser_state	state;

	if (!input)
		return (NULL);
	init_tokenizer(&tokens, &i, &state);
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (!input[i])
			break ;
		len = process_token(input + i, &tokens, env, &state);
		if (!len)
			return (NULL);
		i += len;
	}
	if (state.in_quotes)
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}
