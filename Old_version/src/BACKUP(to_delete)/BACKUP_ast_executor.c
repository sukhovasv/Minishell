#include "minishell.h"

void restore_fd_after_heredoc(t_fd_info *fd_info)
{
    if (fd_info->saved_stdin != -1)
    {
        dup2(fd_info->saved_stdin, STDIN_FILENO);
        close(fd_info->saved_stdin);
        fd_info->saved_stdin = -1;
    }
}

static int handle_command_redirections(t_ast_node *node, t_fd_info *fd_info, t_env *env)
{
    if (node->redirects)
    {
        if (!handle_redirections(node->redirects, fd_info, env, node))
            return 0;
    }
    return 1;
}

static int execute_builtin_command(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int status = execute_builtin(node->args, env);
    restore_redirections(fd_info);
    return status;
}

static int execute_external_command(t_ast_node *node, t_fd_info *fd_info)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
        return 1;
    if (pid == 0)
    {
        setup_child_signals();
        execvp(node->args[0], node->args);
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(node->args[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        exit(127);
    }
    waitpid(pid, &status, 0);
    restore_redirections(fd_info);
    return status / 256; // Это эквивалентно (status >> 8)
}

int execute_command_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int status;

    if (!handle_command_redirections(node, fd_info, env))
        return 0;
        
    if (!node->args || !node->args[0])
        return 0;
        
    if (is_builtin(node->args[0]))
        status = execute_builtin_command(node, env, fd_info);
    else
        status = execute_external_command(node, fd_info);

    restore_fd_after_heredoc(fd_info);  // Добавляем здесь
    return status;
}

static void close_pipe_fds(int pipe_fd[2])
{
    close(pipe_fd[0]);
    close(pipe_fd[1]);
}

/*static int init_pipe_execution(int *pipe_fd, int *original_stdin)
{
    *original_stdin = dup(STDIN_FILENO);
    if (*original_stdin == -1)
    {
        perror("Failed to save original stdin");
        return 0;   // Возвращаем 0 вместо 1 для единообразия (0 = ошибка)
    }

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe failed");
        close(*original_stdin);
        return 0;
    }
    return 1;
}*/

static int init_pipe_execution(int *pipe_fd, int *original_stdin)
{
    printf("DEBUG: Initializing pipe execution\n");
    *original_stdin = dup(STDIN_FILENO);
    if (*original_stdin == -1)
    {
        perror("Failed to save original stdin");
        return 0;
    }
    printf("DEBUG: Original stdin saved: %d\n", *original_stdin);

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe failed");
        close(*original_stdin);
        return 0;
    }
    printf("DEBUG: Pipe created: [%d, %d]\n", pipe_fd[0], pipe_fd[1]);
    return 1;
}

/*static int handle_heredoc_for_pipe(t_ast_node *node, int *heredoc_stdin, 
                                t_env *env, t_fd_info *fd_info, int *pipe_fd, int original_stdin)
{
    if (!node->left || !node->left->redirects)
        return 1;

    t_redirect *redir = node->left->redirects;
    while (redir)
    {
        if (redir->type == TOKEN_REDIR_HEREDOC)
        {
            if (!handle_redir_heredoc(redir, fd_info, env, node->left))
            {
                close_pipe_fds(pipe_fd);
                close(original_stdin);
                return 0;
            }
            *heredoc_stdin = dup(STDIN_FILENO);
            if (*heredoc_stdin == -1)
            {
                close_pipe_fds(pipe_fd);
                close(original_stdin);
                return 0;
            }
            break;
        }
        redir = redir->next;
    }
    return 1;
}*/

static int handle_heredoc_for_pipe(t_ast_node *node, int *heredoc_stdin, 
                                t_env *env, t_fd_info *fd_info, int *pipe_fd, int original_stdin)
{
    printf("DEBUG: Starting handle_heredoc_for_pipe\n");
    if (!node->left || !node->left->redirects)
    {
        printf("DEBUG: No left node or redirects found\n");
        return 1;
    }

    t_redirect *redir = node->left->redirects;
    printf("DEBUG: Checking redirects\n");
    while (redir)
    {
        printf("DEBUG: Found redirect type: %d\n", redir->type);
        if (redir->type == TOKEN_REDIR_HEREDOC)
        {
            printf("DEBUG: Found heredoc, processing...\n");
            if (!handle_redir_heredoc(redir, fd_info, env, node->left))
            {
                printf("DEBUG: Failed to handle heredoc\n");
                close_pipe_fds(pipe_fd);
                close(original_stdin);
                return 0;
            }
            printf("DEBUG: Heredoc handled successfully\n");
            *heredoc_stdin = dup(STDIN_FILENO);
            printf("DEBUG: Heredoc stdin saved as: %d\n", *heredoc_stdin);
            if (*heredoc_stdin == -1)
            {
                perror("Failed to save heredoc fd");
                close_pipe_fds(pipe_fd);
                close(original_stdin);
                return 0;
            }
            return 1;
        }
        redir = redir->next;
    }
    printf("DEBUG: No heredoc found in redirects\n");
    return 1;
}

/*static int handle_heredoc_for_pipe(t_ast_node *node, int *heredoc_stdin, 
                                t_env *env, t_fd_info *fd_info)
{
    printf("DEBUG: Starting handle_heredoc_for_pipe\n");
    t_ast_node *current = node;

    // Ищем самый левый узел с heredoc
    while (current && current->type == AST_PIPE)
    {
        if (current->left && current->left->redirects)
        {
            t_redirect *redir = current->left->redirects;
            while (redir)
            {
                if (redir->type == TOKEN_REDIR_HEREDOC)
                {
                    printf("DEBUG: Found heredoc in left node\n");
                    if (!handle_redir_heredoc(redir, fd_info, env, current->left))
                        return 0;
                    *heredoc_stdin = dup(STDIN_FILENO);
                    if (*heredoc_stdin == -1)
                        return 0;
                    printf("DEBUG: Heredoc saved as fd %d\n", *heredoc_stdin);
                    return 1;
                }
                redir = redir->next;
            }
        }
        current = current->left;
    }
    return 1;
}*/

static int restore_stdin(int original_stdin, int *pipe_fd, int heredoc_stdin)
{
    if (dup2(original_stdin, STDIN_FILENO) == -1)
    {
        close_pipe_fds(pipe_fd);
        if (heredoc_stdin != -1)
            close(heredoc_stdin);
        close(original_stdin);
        return 0;
    }
    return 1;
}

static int create_first_process(pid_t *pid1, int heredoc_stdin, int *pipe_fd)
{
    *pid1 = fork();
    if (*pid1 == -1)
    {
        perror("First fork failed");
        if (heredoc_stdin != -1)
            close(heredoc_stdin);
        close_pipe_fds(pipe_fd);
        return 0;
    }
    return 1;
}

static int handle_first_child(int heredoc_stdin, int *pipe_fd)
{
    close(pipe_fd[0]);
    if (heredoc_stdin != -1)
    {
        if (dup2(heredoc_stdin, STDIN_FILENO) == -1)
        {
            perror("Failed to redirect heredoc stdin");
            close(heredoc_stdin);
            return 0;
        }
        close(heredoc_stdin);
        char buf[1024];
        ssize_t n;
        while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0)
        {
            if (write(pipe_fd[1], buf, n) != n)
            {
                perror("Write to pipe failed");
                return 0;
            }
        }
    }
    return 1;
}

/*static int handle_first_child(int heredoc_stdin, int *pipe_fd)
{
    printf("DEBUG: First child handling heredoc fd: %d\n", heredoc_stdin);
    close(pipe_fd[0]);
    if (heredoc_stdin != -1)
    {
        printf("DEBUG: Setting up heredoc stdin\n");
        if (dup2(heredoc_stdin, STDIN_FILENO) == -1)
        {
            perror("Failed to redirect heredoc stdin");
            close(heredoc_stdin);
            return 0;
        }
        close(heredoc_stdin);
        char buf[1024];
        ssize_t n;
        printf("DEBUG: Reading from heredoc\n");
        while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0)
        {
            printf("DEBUG: Read %zd bytes from heredoc\n", n);
            if (write(pipe_fd[1], buf, n) != n)
            {
                perror("Write to pipe failed");
                return 0;
            }
        }
        if (n < 0)
        {
            perror("Read from heredoc failed");
            return 0;
        }
    }
    printf("DEBUG: First child completed successfully\n");
    return 1;
}*/

static void execute_first_child(int heredoc_stdin, int *pipe_fd)
{
    if (!handle_first_child(heredoc_stdin, pipe_fd))
        exit(1);
    close(pipe_fd[1]);
    exit(0);
}

/*static void execute_first_child(int heredoc_stdin, int *pipe_fd, t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    printf("DEBUG: First child process started\n");
    close(pipe_fd[0]);

    if (heredoc_stdin != -1)
    {
        printf("DEBUG: Setting up heredoc input (fd: %d)\n", heredoc_stdin);
        if (dup2(heredoc_stdin, STDIN_FILENO) == -1)
        {
            perror("Failed to redirect heredoc stdin");
            exit(1);
        }
        close(heredoc_stdin);
    }

    if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
    {
        perror("Failed to redirect stdout to pipe");
        exit(1);
    }
    close(pipe_fd[1]);

    printf("DEBUG: First child executing command: %s\n", 
           node->left && node->left->args ? node->left->args[0] : "unknown");
    int status = execute_ast_node(node->left, env, fd_info);
    printf("DEBUG: Command execution completed with status %d\n", status);
    exit(status);
}*/

/*static void execute_second_child(int pipe_fd, t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    if (dup2(pipe_fd, STDIN_FILENO) == -1)
    {
        perror("Failed to redirect pipe to stdin");
        exit(1);
    }
    close(pipe_fd);
    exit(execute_ast_node(node->right, env, fd_info));
}*/

static void execute_second_child(int pipe_fd, t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    printf("DEBUG: Second child process started\n");
    if (dup2(pipe_fd, STDIN_FILENO) == -1)
    {
        perror("Failed to redirect pipe to stdin");
        exit(1);
    }
    close(pipe_fd);
    printf("DEBUG: Second child executing command\n");
    exit(execute_ast_node(node->right, env, fd_info));
}

static int execute_pipe_processes(pid_t pid1, int *pipe_fd, int heredoc_stdin, 
                                t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int status;
    pid_t pid2;
    
    if (pid1 == 0)
        execute_first_child(heredoc_stdin, pipe_fd);
    close(pipe_fd[1]);
    if (heredoc_stdin != -1)
        close(heredoc_stdin);
    pid2 = fork();
    if (pid2 == 0)
        execute_second_child(pipe_fd[0], node, env, fd_info);
    close(pipe_fd[0]);
    waitpid(pid1, &status, 0);
    int status2;
    waitpid(pid2, &status2, 0);
    if (WIFEXITED(status2))
        return WEXITSTATUS(status2);
    return status2;
}

/*static int execute_pipe_processes(pid_t pid1, int *pipe_fd, int heredoc_stdin, 
                                t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int status;
    
    if (pid1 == 0)  // Первый процесс
    {
        printf("DEBUG: First process executing: %s\n", 
               node->left->args ? node->left->args[0] : "unknown");
        close(pipe_fd[0]);
        
        if (heredoc_stdin != -1)
        {
            if (dup2(heredoc_stdin, STDIN_FILENO) == -1)
                exit(1);
            close(heredoc_stdin);
        }

        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
            exit(1);
        close(pipe_fd[1]);

        exit(execute_ast_node(node->left, env, fd_info));
    }

    // Второй процесс
    pid_t pid2 = fork();
    if (pid2 == -1)
        return 1;

    if (pid2 == 0)
    {
        printf("DEBUG: Second process executing: %s\n", 
               node->right->args ? node->right->args[0] : "unknown");
        close(pipe_fd[1]);
        
        if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
            exit(1);
        close(pipe_fd[0]);

        if (node->right->type == AST_PIPE)
            exit(execute_pipe_node(node->right, env, fd_info));
        else
            exit(execute_ast_node(node->right, env, fd_info));
    }

    // Родительский процесс
    close_pipe_fds(pipe_fd);
    waitpid(pid1, &status, 0);
    
    int status2;
    waitpid(pid2, &status2, 0);

    return WIFEXITED(status2) ? WEXITSTATUS(status2) : status2;
}*/

int execute_pipe_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
   int pipe_fd[2];
   pid_t pid1;
   int heredoc_stdin = -1;
   int original_stdin = dup(STDIN_FILENO);

   if (!init_pipe_execution(pipe_fd, &original_stdin))
    return 1;
   if (!handle_heredoc_for_pipe(node, &heredoc_stdin, env, fd_info, pipe_fd, original_stdin))
    return 1;
   if (!restore_stdin(original_stdin, pipe_fd, heredoc_stdin))
    return 1;
   close(original_stdin);
   if (!create_first_process(&pid1, heredoc_stdin, pipe_fd))
    return 1;
   return execute_pipe_processes(pid1, pipe_fd, heredoc_stdin, node, env, fd_info);
}

/*static int process_heredoc_first(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    t_redirect *redir = node->redirects;
    while (redir)
    {
        if (redir->type == TOKEN_REDIR_HEREDOC)
        {
            if (!handle_redir_heredoc(redir, fd_info, env, node))
                return 0;
            return 1;
        }
        redir = redir->next;
    }
    return 1;
}*/

/*static int is_heredoc_in_pipe(t_ast_node *node)
{
    if (!node || !node->redirects)
        return 0;
        
    t_redirect *redir = node->redirects;
    while (redir)
    {
        if (redir->type == TOKEN_REDIR_HEREDOC)
            return 1;
        redir = redir->next;
    }
    return 0;
}

static int process_pipe_heredoc(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    if (!is_heredoc_in_pipe(node))
        return 1;
        
    // Обрабатываем heredoc до выполнения пайпа
    t_redirect *redir = node->redirects;
    while (redir)
    {
        if (redir->type == TOKEN_REDIR_HEREDOC)
        {
            if (!handle_redir_heredoc(redir, fd_info, env, node))
                return 0;
            break;  // Обрабатываем только первый heredoc в пайпе
        }
        redir = redir->next;
    }
    return 1;
}*/

/*static int prepare_heredoc_content(t_ast_node *node, t_env *env, t_fd_info *fd_info, int *heredoc_pipe)
{
    if (!node || !node->redirects)
        return 1;

    // Создаем пайп для хранения содержимого heredoc
    if (pipe(heredoc_pipe) == -1)
        return 0;

    pid_t pid = fork();
    if (pid == -1)
    {
        close(heredoc_pipe[0]);
        close(heredoc_pipe[1]);
        return 0;
    }

    if (pid == 0)
    {
        // Дочерний процесс
        close(heredoc_pipe[0]); // Закрываем читающий конец
        
        // Перенаправляем stdout в пайп
        if (dup2(heredoc_pipe[1], STDOUT_FILENO) == -1)
            exit(1);
        close(heredoc_pipe[1]);

        // Выполняем команду cat для heredoc
        if (execute_ast_node(node, env, fd_info) != 0)
            exit(1);
        exit(0);
    }

    // Родительский процесс
    close(heredoc_pipe[1]); // Закрываем пишущий конец

    // Ждем завершения обработки heredoc
    int status;
    waitpid(pid, &status, 0);
    
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
    {
        close(heredoc_pipe[0]);
        return 0;
    }

    return 1;
}*/

/*int execute_pipe_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int pipe_fd[2];
    pid_t pid1, pid2;
    int status;
    int heredoc_pipe[2] = {-1, -1}; // Для хранения содержимого heredoc

    // Если есть heredoc в левой команде, обрабатываем его предварительно
    if (node->left && node->left->redirects)
    {
        t_redirect *redir = node->left->redirects;
        while (redir)
        {
            if (redir->type == TOKEN_REDIR_HEREDOC)
            {
                if (!prepare_heredoc_content(node->left, env, fd_info, heredoc_pipe))
                {
                    if (heredoc_pipe[0] != -1) close(heredoc_pipe[0]);
                    return 1;
                }
                break;
            }
            redir = redir->next;
        }
    }

    // Создаем пайп для команд
    if (pipe(pipe_fd) == -1)
    {
        if (heredoc_pipe[0] != -1) close(heredoc_pipe[0]);
        perror("pipe failed");
        return 1;
    }

    pid1 = fork();
    if (pid1 == -1)
    {
        if (heredoc_pipe[0] != -1) close(heredoc_pipe[0]);
        close_pipe_fds(pipe_fd);
        return 1;
    }

    if (pid1 == 0)
    {
        setup_child_signals();
        close(pipe_fd[0]);

        // Если есть heredoc, используем его содержимое
        if (heredoc_pipe[0] != -1)
        {
            if (dup2(heredoc_pipe[0], STDIN_FILENO) == -1)
                exit(1);
            close(heredoc_pipe[0]);
        }

        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
            exit(1);
        close(pipe_fd[1]);

        // Запускаем команду
        exit(execute_ast_node(node->left, env, fd_info));
    }

    pid2 = fork();
    if (pid2 == -1)
    {
        if (heredoc_pipe[0] != -1) close(heredoc_pipe[0]);
        close_pipe_fds(pipe_fd);
        kill(pid1, SIGKILL);
        waitpid(pid1, NULL, 0);
        return 1;
    }

    if (pid2 == 0)
    {
        setup_child_signals();
        if (heredoc_pipe[0] != -1) close(heredoc_pipe[0]);
        close(pipe_fd[1]);

        if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
            exit(1);
        close(pipe_fd[0]);

        if (node->right->type == AST_PIPE)
            exit(execute_pipe_node(node->right, env, fd_info));
        else
            exit(execute_ast_node(node->right, env, fd_info));
    }

    // Закрываем все пайпы в родительском процессе
    if (heredoc_pipe[0] != -1) close(heredoc_pipe[0]);
    close_pipe_fds(pipe_fd);

    // Ждем завершения процессов
    waitpid(pid1, &status, 0);
    int right_status;
    waitpid(pid2, &right_status, 0);

    return WIFEXITED(right_status) ? WEXITSTATUS(right_status) : right_status;
}*/

int execute_ast_redirect_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int status;

    if (node->redirects)
    {
        if (!handle_redirections(node->redirects, fd_info, env, node))
            return 1;
    }
    status = execute_ast_node(node->left, env, fd_info);
    restore_fd_after_heredoc(fd_info);  // Добавляем здесь
    return status;
}

int execute_ast_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    if (!node)
        return 1;
    if (node->type == AST_COMMAND)
        return execute_command_node(node, env, fd_info);
    else if (node->type == AST_PIPE)
        return execute_pipe_node(node, env, fd_info);
    else if (node->type == AST_REDIRECT)
        return execute_ast_redirect_node(node, env, fd_info);
    return (1);
}
