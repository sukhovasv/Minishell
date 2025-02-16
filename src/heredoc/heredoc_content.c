#include "minishell.h"

static void	write_line_to_heredoc(char *line, int fd)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

static char	*handle_variable_expansion(char *line, t_env *env)
{
	char	*expanded;

	if (ft_strchr(line, '$'))
	{
		expanded = expand_env_variables(line, env);
		if (expanded)
		{
			free(line);
			return (expanded);
		}
	}
	return (line);
}

static int	check_heredoc_line(char **line, t_token *token,
			t_env *env, int expand_vars)
{
	if (!(*line) || g_signal_received == SIGINT)
		return (-1);
	if (is_end_of_heredoc(*line, token->value))
	{
		free(*line);
		return (0);
	}
	if (expand_vars)
		*line = handle_variable_expansion(*line, env);
	return (1);
}

static int	cleanup_heredoc(char *line, t_token *token, int fd)
{
	free(line);
	close(fd);
	unlink(token->temp_file);
	free(token->temp_file);
	token->temp_file = NULL;
	return (0);
}

int	handle_heredoc_content(t_token *token, t_env *env)
{
	char	*line;
	int		fd;
	int		expand_vars;
	int		status;

	fd = init_heredoc_file(token);
	if (fd == -1)
		return (0);
	expand_vars = !token->has_quotes;
	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		status = check_heredoc_line(&line, token, env, expand_vars);
		if (status == -1)
			return (cleanup_heredoc(line, token, fd));
		if (status == 0)
			break ;
		write_line_to_heredoc(line, fd);
		free(line);
	}
	close(fd);
	setup_signals();
	return (1);
}
