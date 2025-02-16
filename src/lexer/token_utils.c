#include "minishell.h"

char	*reallocate_result(char *result, char *env_val, size_t j,
			size_t env_len)
{
	char	*new_result;

	new_result = realloc(result, (j + env_len + 2) * sizeof(char));
	if (!new_result)
	{
		free(result);
		free(env_val);
		return (NULL);
	}
	return (new_result);
}

void	copy_env_value(char *result, const char *env_val, size_t *j)
{
	size_t	env_len;

	env_len = ft_strlen(env_val);
	ft_strlcpy(result + *j, env_val, env_len + 1);
	*j += env_len;
}

void	update_index_after_env_var(t_word_parser *parser)
{
	if (parser->str[parser->i + 1] == '?')
		parser->i++;
	else
	{
		parser->i++;
		while (parser->i < parser->len
			&& (ft_isalnum(parser->str[parser->i])
				|| parser->str[parser->i] == '_'))
			parser->i++;
		parser->i--;
	}
}

void	init_tokenizer(t_token **tokens, size_t *i, t_parser_state *state)
{
	*tokens = NULL;
	*i = 0;
	state->in_quotes = 0;
	state->quote_char = 0;
}
