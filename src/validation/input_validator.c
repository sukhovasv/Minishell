#include "minishell.h"

int	is_quote_open(const char *str)
{
	char quote;
	bool is_open;

	quote = 0;
	is_open = false;
	while (*str)
	{
		if ((*str == '\'' || *str == '"') && !quote)
		{
			quote = *str;
			is_open = true;
		}
		else if (*str == quote)
		{
			quote = 0;
			is_open = false;
		}
		str++;
	}
	return (is_open);
}