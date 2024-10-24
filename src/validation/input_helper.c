#include "minishell.h"

char	*trim_str(char *str)
{
	char *const	t_str = ft_strtrim(str, " \t\n\r\v\f");

	if (!t_str)
		return (NULL);
	return (t_str);
}

int	handle_pipe_continuation(char **line_buffer)
{
	char	*next_line;
	char	*combined_input;
	int		len = ft_strlen(*line_buffer);

	if (**line_buffer == '&' || **line_buffer == '|') //TODO error
		return (EXIT_FAILURE);
	if (len > 0 && *line_buffer[len - 1] == '|')
	{
		next_line = readline("> ");
		if (!next_line)
			return (EXIT_SUCCESS);
		combined_input = ft_strjoin(*line_buffer, next_line);
		if (!combined_input)
		{
			free(next_line);
			return (EXIT_SUCCESS);
		}
		free(*line_buffer);
		free(next_line);
		*line_buffer = combined_input;
	}
	return (EXIT_SUCCESS);
}