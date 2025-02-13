#include "minishell.h"

/*int write_single_heredoc(t_heredoc_data *data, t_env *env)
{
    int fd = open_temp_file(data->temp_file);
    if (fd == -1)
        return -1;

    if (read_and_write_lines(fd, data, env) == -1)
    {
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}*/

int write_single_heredoc(t_heredoc_data *data, t_env *env)
{
    int fd = open_temp_file(data->temp_file);
    if (fd == -1)
        return -1;

    if (read_and_write_lines(fd, data, env) == -1)
    {
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

int handle_signals_and_fork(t_heredoc_data *heredocs, int count, t_env *env)
{
    int status;
    pid_t pid;

    pid = fork();
    if (pid == -1)
        return -1;

    if (pid == 0)
    {
        setup_heredoc_signals();  // Специальные сигналы для heredoc
        if (process_heredocs_in_child(heredocs, count, env) != 0)
            exit(1);
        exit(0);
    }

    // Родительский процесс игнорирует сигналы во время ожидания heredoc
    ignore_signals();
    waitpid(pid, &status, 0);
    setup_signals();  // Восстанавливаем нормальную обработку сигналов

    if (g_signal_received == SIGINT)
    {
        g_signal_received = 0;
        return 0;
    }

    return (WIFEXITED(status) && WEXITSTATUS(status) == 0);
}

/*static int is_heredoc_without_command(t_redirect *redir, t_ast_node *ast)
{
    if (!redir) 
        return 1;
    if (ast && ast->args && ast->args[0])  
        return 0;
    return 1;
}*/

static int is_heredoc_without_command(t_redirect *redir, t_ast_node *ast)
{
    if (!redir) 
        return 1;
    
    if (ast && ast->args && ast->args[0] && ast->args[0][0] != '\0')  // ✅ Теперь проверяем, что args не пусты
        return 0;
    
    return 1;
}



/*int handle_redir_heredoc(t_redirect *redir, t_fd_info *fd_info, t_env *env, t_ast_node *ast)
{
    t_heredoc_data *heredocs;
    int count;
    int last_fd;

    count = prepare_heredoc_data(redir, &heredocs);
    if (count == 0)
        return 0;

    last_fd = process_and_open_last_heredoc(heredocs, count, env);
    if (last_fd == -1)
    {
        cleanup_heredoc_files(heredocs, count);
        return 0;
    }
	
	if (is_heredoc_without_command(redir, ast))
    {
        close(last_fd);

        if (heredocs) 
        {
            cleanup_heredoc_files(heredocs, count);
            heredocs = NULL; 
        }
        return (0);
    }
    setup_input_redirection(last_fd, fd_info);
    return 1;
}*/

/*int handle_redir_heredoc(t_redirect *redir, t_fd_info *fd_info, t_env *env, t_ast_node *ast)
{
    t_heredoc_data *heredocs;
    int count;
    int last_fd;

    count = prepare_heredoc_data(redir, &heredocs);
    if (count == 0)
        return 0;

    last_fd = process_and_open_last_heredoc(heredocs, count, env);
    if (last_fd == -1)
        return 0;
    
    if (is_heredoc_without_command(redir, ast))
    {
        close(last_fd);
        return (0);
    }
    setup_input_redirection(last_fd, fd_info);
    return 1;
}*/

/*int handle_redir_heredoc(t_redirect *redir, t_fd_info *fd_info, t_env *env, t_ast_node *ast)
{
    t_heredoc_data *heredocs;
    int count;
    int last_fd;
    
    count = prepare_heredoc_data(redir, &heredocs);
    if (count == 0)
        return 0;

    last_fd = process_and_open_last_heredoc(heredocs, count, env);
    if (last_fd == -1)
        return 0;
    
    if (is_heredoc_without_command(redir, ast))
    {
        close(last_fd);
        return 0;
    }

    // Сохраняем текущий stdin если еще не сохранен
    if (fd_info->saved_stdin == -1)
        fd_info->saved_stdin = dup(STDIN_FILENO);

    // Устанавливаем heredoc как stdin
    if (dup2(last_fd, STDIN_FILENO) == -1)
    {
        close(last_fd);
        return 0;
    }
    close(last_fd);

    return 1;
}*/

int handle_redir_heredoc(t_redirect *redir, t_fd_info *fd_info, t_env *env, t_ast_node *ast)
{
    t_heredoc_data *heredocs;
    int count;
    int last_fd;
    
    count = prepare_heredoc_data(redir, &heredocs);
    if (count == 0)
        return 0;

    last_fd = process_and_open_last_heredoc(heredocs, count, env);
    if (last_fd == -1)
        return 0;
    
    if (is_heredoc_without_command(redir, ast))
    {
        close(last_fd);
        return 0;
    }

    // Сохраняем текущий stdin если еще не сохранен
    if (fd_info->saved_stdin == -1)
        fd_info->saved_stdin = dup(STDIN_FILENO);

    // Устанавливаем heredoc как stdin
    if (dup2(last_fd, STDIN_FILENO) == -1)
    {
        close(last_fd);
        return 0;
    }
    close(last_fd);

    return 1;
}

/*static int should_process_heredoc(t_redirect *redir, t_ast_node *ast)
{
    // Если нет редиректа или AST узла, пропускаем
    if (!redir || !ast) 
        return 0;
    
    // Если это часть пайпа, всегда обрабатываем heredoc
    if (ast->type == AST_PIPE)
        return 1;
        
    // Иначе проверяем наличие команды
    if (ast->args && ast->args[0] && ast->args[0][0] != '\0')
        return 1;
        
    return 0;
}*/

/*int handle_redir_heredoc(t_redirect *redir, t_fd_info *fd_info, t_env *env, t_ast_node *ast)
{
    t_heredoc_data *heredocs = NULL;
    int count;
    int last_fd;
    
    count = prepare_heredoc_data(redir, &heredocs);
    if (count == 0 || !heredocs)
        return 0;

    last_fd = process_and_open_last_heredoc(heredocs, count, env);
    if (last_fd == -1)
    {
        cleanup_heredoc_files(heredocs, count);
        free(heredocs);
        return 0;
    }

    // Проверяем наличие команды и не находимся ли мы в пайпе
    if (!ast->args && !ast->args[0] && ast->type != AST_PIPE)
    {
        close(last_fd);
        cleanup_heredoc_files(heredocs, count);
        free(heredocs);
        return 0;
    }

    // Сохраняем stdin если еще не сохранен
    if (fd_info->saved_stdin == -1)
        fd_info->saved_stdin = dup(STDIN_FILENO);

    // Устанавливаем heredoc как stdin
    if (dup2(last_fd, STDIN_FILENO) == -1)
    {
        perror("Failed to redirect heredoc to stdin");
        close(last_fd);
        cleanup_heredoc_files(heredocs, count);
        free(heredocs);
        return 0;
    }

    close(last_fd);
    cleanup_heredoc_files(heredocs, count);
    free(heredocs);

    return 1;
}*/

/*int handle_redir_heredoc(t_redirect *redir, t_fd_info *fd_info, t_env *env, t_ast_node *ast)
{
    t_heredoc_data *heredocs = NULL;
    int count;
    int last_fd;
    
    count = prepare_heredoc_data(redir, &heredocs);
    if (count == 0 || heredocs == NULL)
        return 0;  // ❗ Если prepare_heredoc_data вернула NULL, ничего не освобождаем

    last_fd = process_and_open_last_heredoc(heredocs, count, env);
    if (last_fd == -1)
    {
        cleanup_heredoc_files(heredocs, count);
        free(heredocs);
        return 0;
    }

    if (is_heredoc_without_command(redir, ast))
    {
        close(last_fd);
        cleanup_heredoc_files(heredocs, count);
        free(heredocs);
        return 0;
    }

    // Сохраняем stdin только один раз
    if (fd_info->saved_stdin == -1)
        fd_info->saved_stdin = dup(STDIN_FILENO);

    if (dup2(last_fd, STDIN_FILENO) == -1)
    {
        perror("Failed to redirect heredoc to stdin");
        close(last_fd);
        cleanup_heredoc_files(heredocs, count);
        free(heredocs);
        return 0;
    }

    close(last_fd);

    // ❗ Освобождаем память только один раз
    if (heredocs)
    {
        cleanup_heredoc_files(heredocs, count);
        free(heredocs);
        heredocs = NULL;  // Защита от double free
    }

    return 1;
}*/
