#include "minishell.h"

static int	is_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '|'
		|| c == '<' || c == '>');
}

static size_t	get_token_length(const char *str, int *in_quotes,
			char *quote_char)
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

static int	handle_heredoc_content(t_token *token, t_env *env)
{
	char	*line;
	int		fd;
	int		expand_vars;
	
	token->temp_file = create_temp_file();
	if (!token->temp_file)
		return 0;
	fd = open(token->temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(token->temp_file);
		token->temp_file = NULL;
		return 0;
	}
	expand_vars = !token->has_quotes;
	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line || g_signal_received == SIGINT)
		{
			free(line);
			close(fd);
			unlink(token->temp_file);
			free(token->temp_file);
			token->temp_file = NULL;
			return (0);
		}
		if (is_end_of_heredoc(line, token->value))
		{
			free(line);
			break;
		}
		if (expand_vars && ft_strchr(line, '$'))
		{
			char *expanded = expand_env_variables(line, env);
			free(line);
			line = expanded;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	setup_signals();  // Восстанавливаем стандартные сигналы
	return (1);
}

/*static int read_and_write_heredoc(t_heredoc_data *data, t_env *env)
{
    char *line;

    while (1)
    {
        line = readline("> ");
        if (!line || g_signal_received == SIGINT)
        {
            free(line);
            close(data->temp_fd);
            unlink(data->temp_file);
            free(data->temp_file);
            data->temp_file = NULL;
            return (0);
        }
        if (is_end_of_heredoc(line, data->delimiter))
        {
            free(line);
            break;
        }
        if (data->expand_vars && ft_strchr(line, '$'))
        {
            char *expanded = expand_env_variables(line, env);
            free(line);
            line = expanded;
        }
        write(data->temp_fd, line, ft_strlen(line));
        write(data->temp_fd, "\n", 1);
        free(line);
    }
    return (1);
}

static int handle_heredoc_content(t_token *token, t_env *env)
{
    t_heredoc_data data;

    token->temp_file = create_temp_file();
    if (!token->temp_file)
        return 0;
    data.temp_fd = open(token->temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (data.temp_fd == -1)
    {
        free(token->temp_file);
        token->temp_file = NULL;
        return 0;
    }
    data.temp_file = token->temp_file;
    data.delimiter = token->value;
    data.expand_vars = !token->has_quotes;
    data.token = token;
    setup_heredoc_signals();
    if (!read_and_write_heredoc(&data, env))
        return (0);
    close(data.temp_fd);
    setup_signals();
    return (1);
}*/

static t_token *new_token(t_token_type type, const char *value, size_t len)
{
    t_token *token;

    token = ft_calloc(1, sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = ft_strndup(value, len);
    if ((value[0] == '\'' && value[len - 1] == '\'') || 
        (value[0] == '"' && value[len - 1] == '"'))
        token->has_quotes = 1;
    else
        token->has_quotes = 0;
    if (!token->value)
    {
        free(token);
        return (NULL);
    }
    token->next = NULL;
    token->heredoc_content = NULL;
    token->temp_file = NULL;
    return (token);
}

static void add_token(t_token **head, t_token *new_token)
{
    t_token *current;

    if (!*head)
    {
        *head = new_token;
        return;
    }
    current = *head;
    while (current->next)
        current = current->next;
    current->next = new_token;
}

static void get_operator_info(const char *str, t_token_type *type, size_t *len)
{
    *len = 1;
    if (str[0] == '|')
        *type = TOKEN_PIPE;
    else if (str[0] == '<')
    {
        if (str[1] == '<')
        {
            *type = TOKEN_REDIR_HEREDOC;
            *len = 2;
        }
        else
            *type = TOKEN_REDIR_IN;
    }
    else if (str[0] == '>')
    {
        if (str[1] == '>')
        {
            *type = TOKEN_REDIR_APPEND;
            *len = 2;
        }
        else
            *type = TOKEN_REDIR_OUT;
    }
}

static size_t handle_operator(const char *str, t_token **tokens, t_env *env)
{
    t_token *token;
    t_token_type type;
    size_t len;
    size_t total_len;

    if (str[0] != '|' && str[0] != '<' && str[0] != '>')
        return (0);

    get_operator_info(str, &type, &len);
    token = new_token(type, str, len);
    if (!token)
        return (0);
    add_token(tokens, token);

    total_len = len;
    if (type == TOKEN_REDIR_HEREDOC)
    {
        // Пропускаем пробелы
        str += len;
        while (*str && (*str == ' ' || *str == '\t'))
        {
            str++;
            total_len++;
        }

        // Получаем делимитер
        int in_quotes = 0;
        char quote_char = 0;
        size_t delim_len = get_token_length(str, &in_quotes, &quote_char);
        if (delim_len == 0)
            return 0;

        t_token *delim_token = new_token(TOKEN_WORD, str, delim_len);
        if (!delim_token)
            return 0;

        add_token(tokens, delim_token);
        if (!handle_heredoc_content(delim_token, env))
            return 0;

        total_len += delim_len;
    }

    return total_len;
}

size_t handle_word(const char *str, t_token **tokens, t_env *env, t_parser_state *state)
{
    size_t len;
    t_token *token;
    
    len = get_token_length(str, &state->in_quotes, &state->quote_char);
    if (len == 0)
        return 0;

    if (str[0] == '$')
    {
        char *expanded = get_env_value(str, env);
        if (!expanded)
            return 0;
        token = new_token(TOKEN_WORD, expanded, ft_strlen(expanded));
        free(expanded);
    }
    else
        token = new_token(TOKEN_WORD, str, len);

    if (!token)
        return 0;

    add_token(tokens, token);
    return len;
}

static size_t process_token(const char *input, t_token **tokens, t_env *env, t_parser_state *state)
{
    size_t len;

    if (input[0] == '|' || input[0] == '<' || input[0] == '>')
        len = handle_operator(input, tokens, env);
    else
        len = handle_word(input, tokens, env, state);

    if (!len)
    {
        free_tokens(*tokens);
        *tokens = NULL;
        return 0;
    }
    return len;
}

t_token *tokenize(const char *input, t_env *env)
{
    t_token *tokens;
    size_t i;
    size_t len;
    t_parser_state state;

    if (!input)
        return NULL;

    tokens = NULL;
    i = 0;
    state.in_quotes = 0;
    state.quote_char = 0;

    while (input[i])
    {
        while (input[i] && (input[i] == ' ' || input[i] == '\t'))
            i++;
        if (!input[i])
            break;

        len = process_token(input + i, &tokens, env, &state);
        if (!len)
            return NULL;
        i += len;
    }

    if (state.in_quotes)
    {
        free_tokens(tokens);
        return NULL;
    }

    return tokens;
}

void free_tokens(t_token *tokens)
{
    t_token *next;

    while (tokens)
    {
        next = tokens->next;
        free(tokens->value);
        if (tokens->heredoc_content)
            free(tokens->heredoc_content);
        if (tokens->temp_file)
        {
            unlink(tokens->temp_file);
            free(tokens->temp_file);
        }
        free(tokens);
        tokens = next;
    }
}
