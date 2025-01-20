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

char *expand_env_variables(const char *input, t_env *env)
{
    char *result;
    char *temp;
    size_t i = 0;
    size_t input_len = ft_strlen(input);
    
    result = ft_strdup("");  // Начинаем с пустой строки
    if (!result)
        return NULL;

    while (i < input_len)
    {
        if (input[i] == '$' && (ft_isalpha(input[i + 1]) || input[i + 1] == '_'))
        {
            char *expanded_var = expand_env_var(input, &i, env); // Раскрываем $VAR
            temp = ft_strjoin(result, expanded_var);
            free(expanded_var);
            free(result);
            result = temp;
        }
        else
        {
            temp = ft_strndup(&input[i], 1);
            char *new_result = ft_strjoin(result, temp);
            free(temp);
            free(result);
            result = new_result;
            i++;
        }
    }
    return result;
}


int read_and_write_lines(int fd, t_heredoc_data *heredoc, t_env *env)
{
    char *line;
    int expand_vars = 1;

    // Проверяем, были ли кавычки в delimiter
    if (heredoc->token && heredoc->token->has_quotes)
        expand_vars = 0;

    while (1)
    {
        line = readline("> ");
        if (!line || g_signal_received == SIGINT)
        {
            if (line)
                free(line);
            return -1;
        }
        if (is_end_of_heredoc(line, heredoc->delimiter))
        {
            free(line);
            break;
        }
        if (expand_vars && ft_strchr(line, '$'))  
        {
            char *expanded_line = expand_env_variables(line, env);
            free(line);
            line = expanded_line;
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
