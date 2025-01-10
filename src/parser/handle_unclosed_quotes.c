#include "minishell.h"

static void init_check_state(t_quote_state *state)
{
    state->in_quotes = 0;
    state->quote_type = 0;
}

static void open_quotes(t_quote_state *state, char quote)
{
    state->in_quotes = 1;
    state->quote_type = quote;
}

static void close_quotes(t_quote_state *state)
{
    state->in_quotes = 0;
    state->quote_type = 0;
}

static int check_unclosed_quotes(const char *str)
{
    t_quote_state state;
    int i;

    init_check_state(&state);
    i = 0;
    while (str[i])
    {
        if ((str[i] == '"' || str[i] == '\'') && !state.in_quotes)
            open_quotes(&state, str[i]);
        else if (state.in_quotes && str[i] == state.quote_type)
            close_quotes(&state);
        else if ((str[i] == '"' || str[i] == '\'') &&
                state.in_quotes && str[i] != state.quote_type)
            continue;
        i++;
    }
    return (state.in_quotes);
}

static void init_quote_state(t_quote_state *state)
{
    state->in_quotes = 0;
    state->quote_type = 0;
    state->capacity = 1024;
    state->length = 0;
    state->result = malloc(state->capacity);
    if (state->result)
        state->result[0] = '\0';
}

static int append_to_result(t_quote_state *state, const char *str)
{
    size_t len;
    char *new_buf;

    len = ft_strlen(str);
    if (state->length + len + 2 >= state->capacity)
    {
        state->capacity = (state->length + len + 2) * 2;
        new_buf = malloc(state->capacity);
        if (!new_buf)
            return (0);
        ft_strlcpy(new_buf, state->result, state->capacity);
        free(state->result);
        state->result = new_buf;
    }
    if (state->length > 0)
    {
        state->result[state->length] = '\n';
        state->length++;
    }
    ft_strlcpy(state->result + state->length, str, state->capacity - state->length);
    state->length += len;
    return (1);
}

static void open_quote(t_quote_state *state, char input_char, char *current_quote)
{
    state->in_quotes = 1;
    state->quote_type = input_char;
    *current_quote = input_char;
}

static void close_quote(t_quote_state *state, char *current_quote)
{
    state->in_quotes = 0;
    state->quote_type = 0;
    *current_quote = 0;
}

static int process_first_line(t_quote_state *state, const char *input)
{
    int i;
    char current_quote;

    if (!append_to_result(state, input))
        return (0);
    i = 0;
    current_quote = 0;
    while (input[i])
    {
        if ((input[i] == '"' || input[i] == '\'') && !state->in_quotes)
            open_quote(state, input[i], &current_quote);
        else if (state->in_quotes && input[i] == current_quote)
            close_quote(state, &current_quote);
        else if ((input[i] == '"' || input[i] == '\'') &&
                state->in_quotes && input[i] != current_quote)
            continue;
        i++;
    }
    return (1);
}

// Функция для обработки одной строки продолжения
static int process_continuation_line(t_quote_state *state)
{
    char *line;
    int i;

    line = readline("> ");
    if (!line)
        return (0);
    if (!append_to_result(state, line))
    {
        free(line);
        return (0);
    }
    i = 0;
    while (line[i])
    {
        if (line[i] == state->quote_type)
            state->in_quotes = !state->in_quotes;
        i++;
    }
    free(line);
    return (1);
}

// Функция для обработки всех строк продолжения
static char *process_continuation_lines(t_quote_state *state)
{
    while (state->in_quotes)
    {
        if (!process_continuation_line(state))
        {
            free(state->result);
            return (NULL);
        }
    }
    return (state->result);
}

// Основная функция
char *handle_unclosed_quotes(const char *input)
{
    t_quote_state state;

    if (!check_unclosed_quotes(input))
        return (ft_strdup(input));
    init_quote_state(&state);
    if (!state.result)
        return (NULL);
    if (!process_first_line(&state, input))
    {
        free(state.result);
        return (NULL);
    }
    return process_continuation_lines(&state);
}
