#include "minishell.h"

static char	*process_value_with_quotes(char *value)
{
	size_t	len;

	len = ft_strlen(value);
	if ((value[0] == '"' || value[0] == '\'') && value[len - 1] == value[0])
	{
		value[len - 1] = '\0';
		value++;
	}
	return (value);
}

static char	*process_value_without_quotes(char *value)
{
	char	*space_pos;

	if (!value || value[0] == '\0')
		return (NULL);
	space_pos = strchr(value, ' ');
	if (space_pos)
		*space_pos = '\0';
	return (value);
}

char	*process_value(char *value)
{
	if ((value[0] == '"' || value[0] == '\'') && value[strlen(value) - 1]
		== value[0])
		return (process_value_with_quotes(value));
	return (process_value_without_quotes(value));
}
