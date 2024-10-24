#include "minishell.h"

bool	update_quote_state(char c, bool *quote_flag, char *quote_type)
{
	if (!*quote_flag && (c == '\'' || c == '"'))
	{
		*quote_flag = true;
		*quote_type = c;
	}
	else if (*quote_flag && c == *quote_type)
	{
		*quote_flag = false;
	}
	return *quote_flag;
}

int	is_delimiter_char(char current_char)
{
	return (ft_strchr(" \t\n\r\v\f<|>", current_char) != NULL);
}

char	*parse_phrase(char *str, char **start)
{
	bool quote_flag = false;
	char quote_type = 0;

	*start = str;
	while (*str)
	{
		update_quote_state(*str, &quote_flag, &quote_type);
		if (!quote_flag && is_delimiter_char(*str))
			break;
		str++;
	}
	return str;
}

char	*tokenise_phrase(char *str, t_token **tokens)
{
	char	*start;

	str = parse_phrase(str, &start);
	append_word_token(start, str, tokens);
	return (str);
}