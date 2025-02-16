#include "minishell.h"

int	is_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '|'
		|| c == '<' || c == '>');
}

size_t	get_word_length(const char *str, int *in_quotes, char *quote_char)
{
	size_t	len;

	len = 0;
	while (str[len] && (!is_delimiter(str[len]) || *in_quotes))
	{
		if ((str[len] == '"' || str[len] == '\'') && !(*in_quotes))
		{
			*in_quotes = 1;
			*quote_char = str[len];
		}
		else if (*in_quotes && str[len] == *quote_char)
			*in_quotes = 0;
		len++;
	}
	return (len);
}

t_token	*new_token(t_token_type type, const char *value, size_t len)
{
	t_token	*token;
	char	*processed_value;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	processed_value = process_token_value(value, len, &token->has_quotes);
	if (!processed_value)
	{
		free(token);
		return (NULL);
	}
	token->value = processed_value;
	token->next = NULL;
	token->heredoc_content = NULL;
	token->temp_file = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

void	free_tokens(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = next;
	}
}
