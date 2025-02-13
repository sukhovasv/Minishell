#include "minishell.h"

int process_current_heredoc(t_heredoc_data *heredocs, t_redirect *current, int i)
{
    if (!initialize_single_heredoc(&heredocs[i], current))
    {
        free_initialized_heredocs(heredocs, i);
        return 0;
    }
    return 1;
}

int process_and_open_last_heredoc(t_heredoc_data *heredocs, int count, t_env *env)
{
    int last_fd;

    if (!handle_signals_and_fork(heredocs, count, env))
    {
        cleanup_heredoc_files(heredocs, count);
        return -1;
    }

    // Открываем последний heredoc для чтения
    last_fd = open(heredocs[count - 1].temp_file, O_RDONLY);
    if (last_fd == -1)
    {
        cleanup_heredoc_files(heredocs, count);
        return -1;
    }

    return last_fd;
}

int fork_and_process_heredocs(t_heredoc_data *heredocs, int count, t_env *env)
{
    int i;
    
    for (i = 0; i < count; i++)
    {
        if (write_single_heredoc(&heredocs[i], env) != 0)
            return -1;
    }
    
    return 0;
}

int process_heredocs_in_child(t_heredoc_data *heredocs, int count, t_env *env)
{
    setup_heredoc_signals();
    return fork_and_process_heredocs(heredocs, count, env);
}

/*int	handle_heredoc_content(t_token *token, t_env *env)
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
}*/

int handle_heredoc_content(t_token *token, t_env *env)
{
    char *line;
    int fd;
    int expand_vars;

    ft_putstr_fd("\n[DEBUG] Starting heredoc, token value: ", 2);
    ft_putstr_fd(token->value, 2);
    ft_putstr_fd("\n", 2);
    ft_putstr_fd("[DEBUG] has_quotes: ", 2);
    ft_putchar_fd(token->has_quotes ? '1' : '0', 2);
    ft_putstr_fd("\n", 2);

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

    ft_putstr_fd("[DEBUG] expand_vars: ", 2);
    ft_putchar_fd(expand_vars ? '1' : '0', 2);
    ft_putstr_fd("\n", 2);

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

        ft_putstr_fd("[DEBUG] Read line: ", 2);
        ft_putstr_fd(line, 2);
        ft_putstr_fd("\n", 2);

        if (is_end_of_heredoc(line, token->value))
        {
            free(line);
            break;
        }

        if (expand_vars && ft_strchr(line, '$'))
        {
            ft_putstr_fd("[DEBUG] Expanding variables in line\n", 2);
            char *expanded = expand_env_variables(line, env);
            if (expanded)
            {
                ft_putstr_fd("[DEBUG] After expansion: ", 2);
                ft_putstr_fd(expanded, 2);
                ft_putstr_fd("\n", 2);
                free(line);
                line = expanded;
            }
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
    setup_signals();
    return (1);
}
