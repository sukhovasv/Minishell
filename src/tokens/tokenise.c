#include "minishell.h"

t_token	*tokenise(char *str)
{
	t_token	*tokens;

	tokens = NULL;
	while (*str)
	{
		while (*str && ft_strchr(" \t\n\r\v\f", *str) != NULL)
			str++;
		if (ft_strchr("<|>", *str) != NULL)
			str = tokenise_special_chars(str, &tokens);
		else
			str = tokenise_phrase(str, &tokens);
	}
	return (tokens);
}
