#include "minishell.h"

char	*process_token_value(const char *value, size_t len, int *has_quotes)
{
	char	*processed_value;

	if ((value[0] == '\'' && value[len - 1] == '\'')
		|| (value[0] == '"' && value[len - 1] == '"'))
	{
		*has_quotes = 1;
		processed_value = ft_strndup(value + 1, len - 2);
	}
	else
	{
		*has_quotes = 0;
		processed_value = ft_strndup(value, len);
	}
	return (processed_value);
}

t_token	*handle_env_token(const char *str, t_env *env)
{
	char	*value;
	t_token	*token;

	value = get_env_value(str, env);
	if (!value)
		return (NULL);
	token = new_token(TOKEN_WORD, value, ft_strlen(value));
	free(value);
	return (token);
}

t_token	*handle_regular_token(const char *str, size_t len, t_env *env)
{
	char	*value;
	t_token	*token;

	value = create_word_without_quotes(str, len, env);
	if (!value)
		return (NULL);
	token = new_token(TOKEN_WORD, value, ft_strlen(value));
	free(value);
	return (token);
}

int	is_last_token_heredoc(t_token *tokens)
{
	t_token	*last;

	last = NULL;
	if (!tokens)
		return (0);
	last = tokens;
	while (last->next)
		last = last->next;
	return (last->type == TOKEN_REDIR_HEREDOC);
}

int	has_paired_quotes(const char *str, size_t len)
{
	char	quote_char;
	size_t	i;
	int		found_first;

	i = 0;
	found_first = 0;
	quote_char = 0;
	while (i < len)
	{
		if ((str[i] == '"' || str[i] == '\'') && !found_first)
		{
			found_first = 1;
			quote_char = str[i];
		}
		else if (found_first && str[i] == quote_char)
			return (1);
		i++;
	}
	return (0);
}
