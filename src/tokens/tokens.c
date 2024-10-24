#include "minishell.h"

t_token	*init_token(char *value, t_token_type type)
{
	t_token	*const token = ft_calloc(1, sizeof(t_token));

	if (!token || !value)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

void	append_token(t_token **list, t_token *new_token)
{
	t_token	*current;

	if (!*list)
		*list = new_token;
	else
	{
		current = *list;
		while (current->next)
			current = current->next;
		current->next = new_token;
		new_token->prev = current;
	}
}

char	*extract_token_value(char *start, char *end)
{
	if (end > start)
		return (ft_strndup(start, end - start));
	return (NULL);
}

void	append_word_token(char *start, char *end, t_token **token_list)
{
	char	*const token_value = extract_token_value(start, end);

	if (token_value)
	{
		append_token(token_list, init_token(token_value, WORD));
		free(token_value);
	}
	else
		ft_printf("Error: token memory allocation\n");
}