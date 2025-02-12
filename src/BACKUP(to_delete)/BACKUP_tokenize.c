#include "minishell.h"

static int is_delimiter(char c)
{
    return (c == ' ' || c == '\t' || c == '|' || 
            c == '<' || c == '>');
}

static size_t get_word_length(const char *str, int *in_quotes, char *quote_char)
{
    size_t len;

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
        token->has_quotes = 1; // Сохраняем, что токен был в кавычках
    else
        token->has_quotes = 0;
    if (!token->value)
    {
        free(token);
        return (NULL);
    }
    token->next = NULL;
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

static size_t handle_operator(const char *str, t_token **tokens)
{
    t_token *token;
    t_token_type type;
    size_t len;

    if (str[0] != '|' && str[0] != '<' && str[0] != '>')
        return (0);
    get_operator_info(str, &type, &len);
    token = new_token(type, str, len);
    if (!token)
        return (0);
    add_token(tokens, token);
    return (len);
}

static char *handle_special_env(const char *str, t_env *env)
{
    char *status;
    
    if (!str || !env)
        return (NULL);
    if (str[0] == '?')  // встретили ?
    {
        status = ft_itoa(env->last_status);
        if (!status)
            return (ft_strdup("0"));
        while (str[1])  // пропускаем все символы после ? если они есть
            str++;
        return (status);
    }
    return (NULL);
}

static char *find_env_value(const char *var_name, size_t len, t_env *env)
{
    int i;

    i = 0;
    while (env->environ[i])
    {
        // Сначала проверяем длину имени переменной
        size_t env_name_len = 0;
        while (env->environ[i][env_name_len] && env->environ[i][env_name_len] != '=')
            env_name_len++;
            
        // Сравниваем только если длины совпадают
        if (env_name_len == len && 
            ft_strncmp(env->environ[i], var_name, len) == 0)
        {
            return ft_strdup(env->environ[i] + len + 1);
        }
        i++;
    }
    return (ft_strdup(""));
}

char *get_env_value(const char *str, t_env *env)
{
    char *value;
    char *var_name;
    size_t len;

    if (!str || !env || !env->environ)
        return (NULL);
    str++;  // пропускаем $
    value = handle_special_env(str, env);
    if (value)
        return (value);
    len = 0;
    while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
        len++;
    if (len == 0)
        return (ft_strdup("$"));
    var_name = ft_strndup(str, len);
    if (!var_name)
        return (NULL);
    value = find_env_value(var_name, len, env);
    free(var_name);
    if (value)
    	return (value);
    else
        return (ft_strdup(""));
}

static int handle_opening_quote(const char *str, size_t *i, int *in_quotes, char *quote_char)
{
    if ((str[*i] == '"' || str[*i] == '\'') && !(*in_quotes))
    {
        *quote_char = str[*i];
        *in_quotes = 1;
        (*i)++;
        return (1);
    }
    return (0);
}

static int handle_closing_quote(const char *str, size_t *i, int *in_quotes, char *quote_char)
{
    if (*in_quotes && str[*i] == *quote_char)
    {
        *in_quotes = 0;
        *quote_char = 0;
        (*i)++;
        return (1);
    }
    return (0);
}

static char *reallocate_result(char *result, char *env_val, size_t j, size_t env_len)
{
    char *new_result = realloc(result, (j + env_len + 2) * sizeof(char));
    if (!new_result)
    {
        free(result);
        free(env_val);
        return (NULL);
    }
    return (new_result);
}

static void copy_env_value(char *result, const char *env_val, size_t *j)
{
    size_t env_len = ft_strlen(env_val);
    ft_strlcpy(result + *j, env_val, env_len + 1);
    *j += env_len;
}

static char *handle_env_var(const char *str, size_t *i, char *result, size_t *j, 
                          size_t len, t_env *env)
{
    char *env_val = get_env_value(str + *i, env);
    if (!env_val)
        return (result);

    size_t env_len = ft_strlen(env_val);
    if (*j + env_len >= len * 2)
    {
        result = reallocate_result(result, env_val, *j, env_len);
        if (!result)
            return (NULL);
    }
    copy_env_value(result, env_val, j);
    free(env_val);
    
    // Пропускаем имя переменной целиком
    if (str[*i + 1] == '?')
        (*i)++;
    else
    {
        (*i)++; // Пропускаем $
        while (*i < len && (ft_isalnum(str[*i]) || str[*i] == '_'))
            (*i)++;
        (*i)--; // Компенсируем i++ в вызывающей функции
    }
    
    return (result);
}

static char *process_word_content(const char *str, size_t *i, char *result, 
    size_t *j, size_t len, t_env *env, int *in_quotes, char *quote_char)
{
    while (*i < len)
    {
        // Обработка кавычек
        if (handle_opening_quote(str, i, in_quotes, quote_char))
            continue;
        if (handle_closing_quote(str, i, in_quotes, quote_char))
            continue;

        // Обработка переменной окружения
        if (str[*i] == '$' && *quote_char != '\'')
        {
            result = handle_env_var(str, i, result, j, len, env);
            if (!result)
                return (NULL);
            (*i)++;  // Явно инкрементируем после обработки
            continue;
        }

        // Обработка обычного символа
        result[*j] = str[*i];
        (*j)++;
        (*i)++;
    }
    return (result);
}

static char *create_word_without_quotes(const char *str, size_t len, t_env *env)
{
    char *result;
    size_t i;
    size_t j;
    int in_quotes;
    char quote_char;

    result = malloc(len * 2 + 1);
    if (!result)
        return (NULL);
    i = 0;
    j = 0;
    in_quotes = 0;
    quote_char = 0;
    result = process_word_content(str, &i, result, &j, len, env, 
        &in_quotes, &quote_char);
    if (result)
        result[j] = '\0';
    return (result);
}

static t_token *handle_env_token(const char *str, t_env *env)
{
    char *value;
    t_token *token;

    value = get_env_value(str, env);
    if (!value)
        return (NULL);
    token = new_token(TOKEN_WORD, value, ft_strlen(value));
    free(value);
    return (token);
}

static t_token *handle_regular_token(const char *str, size_t len, t_env *env)
{
    char *value;
    t_token *token;

    value = create_word_without_quotes(str, len, env);
    if (!value)
        return (NULL);
    token = new_token(TOKEN_WORD, value, ft_strlen(value));
    free(value);
    return (token);
}

static int is_last_token_heredoc(t_token *tokens)
{
    t_token *last = NULL;
    
    if (!tokens)
        return 0;
    last = tokens;
    while (last->next)
        last = last->next;
    return (last->type == TOKEN_REDIR_HEREDOC);
}

static int has_paired_quotes(const char *str, size_t len)
{
    char quote_char;
    size_t i;  // Изменили тип с int на size_t
    int found_first;

    i = 0;
    found_first = 0;
    quote_char = 0;
    while (i < len)
    {
        if ((str[i] == '"' || str[i] == '\'') && !found_first)
        {
            found_first = 1;
            quote_char = str[i];
        }
        else if (found_first && str[i] == quote_char)
            return (1);
        i++;
    }
    return (0);
}

static size_t handle_word(const char *str, t_token **tokens, t_env *env, t_parser_state *state)
{
    size_t len;
    t_token *token;

    len = get_word_length(str, &state->in_quotes, &state->quote_char);
    if (len == 0)
        return (0);
    if (str[0] == '$')
        token = handle_env_token(str, env);
    else
    {
        token = handle_regular_token(str, len, env);
        // Если предыдущий токен был heredoc, проверяем наличие парных кавычек где угодно
        if (token && is_last_token_heredoc(*tokens))
        {
            token->has_quotes = has_paired_quotes(str, len);
        }
    }
    if (!token)
        return (0);
    add_token(tokens, token);
    return (len);
}

static size_t process_token(const char *input, t_token **tokens, t_env *env, t_parser_state *state)
{
    size_t len;

    if (input[0] == '|' || input[0] == '<' || input[0] == '>')
        len = handle_operator(input, tokens);
    else
        len = handle_word(input, tokens, env, state);
    if (!len)
    {
        free_tokens(*tokens);
        *tokens = NULL;
        return (0);
    }
    return (len);
}

t_token *tokenize(const char *input, t_env *env)
{
    t_token *tokens;
    size_t i;
    size_t len;
    t_parser_state state;

    if (!input)
        return (NULL);
    tokens = NULL;
    i = 0;
    state.in_quotes = 0;
    state.quote_char = 0;
    
    while (input[i])
    {
        while (input[i] == ' ' || input[i] == '\t')
            i++;
        if (!input[i])
            break;
        len = process_token(input + i, &tokens, env, &state);
        if (!len)
            return (NULL);
        i += len;
    }
    if (state.in_quotes) // Проверка на незакрытые кавычки
    {
        free_tokens(tokens);
        return (NULL);
    }
    return (tokens);
}

void free_tokens(t_token *tokens)
{
    t_token *next;

    while (tokens)
    {
        next = tokens->next;
        free(tokens->value);
        free(tokens);
        tokens = next;
    }
}
