#include "minishell.h"

/*void	get_operator_info(const char *str, t_token_type *type, size_t *len)
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
}*/

/*size_t handle_operator(const char *str, t_token **tokens, t_env *env)
{
    t_token *token;
    t_token_type type;
    size_t len;
    size_t total_len;
	int in_quotes;
	char quote_char;


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
        in_quotes = 0;
        quote_char = 0;
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
}*/

/*size_t handle_word(const char *str, t_token **tokens, t_env *env, t_parser_state *state)
{
    size_t len;
    t_token *token;
    char *expanded;
    char *result;
    size_t i = 0;
    int in_single = 0;
    int in_double = 0;
    char *debug_str;

    ft_putstr_fd("\n[DEBUG] handle_word called with: ", 2);
    debug_str = ft_strdup(str);
    if (debug_str)
    {
        ft_putstr_fd(debug_str, 2);
        free(debug_str);
    }
    ft_putstr_fd("\n", 2);

    len = get_token_length(str, &state->in_quotes, &state->quote_char);
    if (len == 0)
        return (0);

    result = malloc(len + 1);
    if (!result)
        return (0);
    
    size_t j = 0;
    while (i < len)
    {
        if (str[i] == '\'' && !in_double)
        {
            in_single = !in_single;
            i++;
            continue;
        }
        else if (str[i] == '"' && !in_single)
        {
            in_double = !in_double;
            i++;
            continue;
        }
        else if (str[i] == '$' && !in_single)
        {
            size_t var_start = i + 1;
            size_t var_len = 0;
            i++;
            while (i < len && (ft_isalnum(str[i]) || str[i] == '_'))
            {
                var_len++;
                i++;
            }
            if (var_len > 0)
            {
                char *var_name = ft_strndup(&str[var_start], var_len);
                if (var_name)
                {
                    expanded = get_env_value(var_name, env);
                    free(var_name);
                    if (expanded)
                    {
                        // Используем ft_strlcpy вместо ft_strcpy
                        ft_strlcpy(&result[j], expanded, len - j + 1);
                        j += ft_strlen(expanded);
                        free(expanded);
                        continue;
                    }
                }
            }
            i = var_start - 1;
        }
        result[j++] = str[i++];
    }
    result[j] = '\0';

    token = new_token(TOKEN_WORD, result, ft_strlen(result));
    free(result);

    if (!token)
        return (0);

    add_token(tokens, token);
    return (len);
}*/

/*size_t	process_token(const char *input, t_token **tokens, t_env *env,
		t_parser_state *state)
{
	size_t	len;

	if (input[0] == '|' || input[0] == '<' || input[0] == '>')
		len = handle_operator(input, tokens, env);
	else
		len = handle_word(input, tokens, env, state);
	if (!len)
	{
		free_tokens(*tokens);
		*tokens = NULL;
		return (0);
	}
	return (len);
}*/

/*void	free_tokens(t_token *tokens)
{
	t_token	*next;

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
}*/

t_token *create_heredoc_token(const char *str, size_t len, int *expand_vars)
{
    t_token *token;
    char *cleaned_delim;

    token = new_token(TOKEN_WORD, str, len);
    if (!token)
        return NULL;

    // Очищаем делимитер от кавычек
    cleaned_delim = clean_delimiter(token->value);
    if (!cleaned_delim)
    {
        free_tokens(token);
        return NULL;
    }

    // Проверяем, нужно ли раскрывать переменные
    *expand_vars = should_expand_heredoc_vars(str);

    // Обновляем значение токена
    free(token->value);
    token->value = cleaned_delim;
    
    return token;
}
