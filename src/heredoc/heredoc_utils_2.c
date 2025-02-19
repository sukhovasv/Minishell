#include "minishell.h"

int	has_any_quotes(const char *str)
{
	while (*str)
	{
		if (*str == '\'' || *str == '"')
			return (1);
		str++;
	}
	return (0);
}

int	should_expand_heredoc_vars(const char *delimiter)
{
	if (!delimiter || !*delimiter)
		return (1);
	if (has_any_quotes(delimiter))
		return (0);
	return (1);
}

char	*clean_delimiter(const char *str)
{
	char	*result;
	size_t	i;
	size_t	j;
	size_t	len;

	i = 0;
	j = 0;
	len = ft_strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '"')
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

/*int	init_heredoc_file(t_token *token)
{
	int	fd;

	if (token->temp_file)
		free(token->temp_file);
	token->temp_file = create_temp_file();
	if (!token->temp_file)
		return (-1);
	fd = open(token->temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(token->temp_file);
		token->temp_file = NULL;
		return (-1);
	}
	return (fd);
}*/

int init_heredoc_file(t_token *token)
{
    int fd;

    ft_putstr_fd("\nDebug: Starting init_heredoc_file\n", 2);

    if (!token)
    {
        ft_putstr_fd("Debug: Token is NULL\n", 2);
        return (-1);
    }

    ft_putstr_fd("Debug: Checking existing temp_file\n", 2);
    if (token->temp_file)
    {
        ft_putstr_fd("Debug: Freeing existing temp_file\n", 2);
        free(token->temp_file);
    }

    ft_putstr_fd("Debug: Creating new temp_file\n", 2);
    token->temp_file = create_temp_file();
    if (!token->temp_file)
    {
        ft_putstr_fd("Debug: Failed to create temp_file\n", 2);
        return (-1);
    }

    ft_putstr_fd("Debug: Opening temp_file: ", 2);
    ft_putstr_fd(token->temp_file, 2);
    ft_putstr_fd("\n", 2);

    fd = open(token->temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd == -1)
    {
        ft_putstr_fd("Debug: Failed to open temp_file\n", 2);
        free(token->temp_file);
        token->temp_file = NULL;
        return (-1);
    }

    ft_putstr_fd("Debug: Successfully created heredoc file\n", 2);
    return (fd);
}
