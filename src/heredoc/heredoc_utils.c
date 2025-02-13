#include "minishell.h"

char *create_temp_file(void)
{
    static int  temp_count = 0;
    char        *temp_name;
    char        *num_str;

    num_str = ft_itoa(temp_count++);
    if (!num_str)
        return (NULL);
    temp_name = ft_strjoin("/tmp/heredoc_", num_str);
    free(num_str);
    return (temp_name);
}

int open_temp_file(const char *temp_file)
{
    int fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd == -1)
    {
        perror("Error opening file");
        return -1;
    }
    return fd;
}

int is_end_of_heredoc(const char *line, const char *delimiter)
{
    return (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0 &&
            ft_strlen(line) == ft_strlen(delimiter));
}

int read_and_write_lines(int fd, t_heredoc_data *data, t_env *env)
{
    char *line;
    int expand_vars;

    expand_vars = !(data->token && data->token->has_quotes);

    while (1)
    {
        line = readline("> ");
        if (!line || g_signal_received == SIGINT)
        {
            free(line);
            return -1;
        }

        if (is_end_of_heredoc(line, data->delimiter))
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
    return 0;
}

void setup_input_redirection(int last_fd, t_fd_info *fd_info)
{
    if (fd_info->saved_stdin == -1)
        fd_info->saved_stdin = dup(STDIN_FILENO);
    dup2(last_fd, STDIN_FILENO);
    close(last_fd);
}

// Проверяет, содержит ли строка кавычки где-либо (не обязательно в начале/конце)
int has_any_quotes(const char *str)
{
    while (*str)
    {
        if (*str == '\'' || *str == '"')
            return 1;
        str++;
    }
    return 0;
}

// Проверяет, нужно ли раскрывать переменные в heredoc
int should_expand_heredoc_vars(const char *delimiter)
{
    // Если делимитер пустой
    if (!delimiter || !*delimiter)
        return 1;

    // Если делимитер обрамлен кавычками или содержит кавычки где-либо
    if (has_any_quotes(delimiter))
        return 0;

    return 1;
}

// Очищает делимитер от кавычек
char *clean_delimiter(const char *str)
{
    char *result;
    size_t i = 0;
    size_t j = 0;
    size_t len = ft_strlen(str);

    result = malloc(len + 1);
    if (!result)
        return NULL;

    while (str[i])
    {
        if (str[i] != '\'' && str[i] != '"')
            result[j++] = str[i];
        i++;
    }
    result[j] = '\0';
    return result;
}
