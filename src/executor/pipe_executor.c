#include "minishell.h"

int execute_pipe_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int pipefd[2];
    pid_t pid1, pid2;
    int status;

    ft_putstr_fd("\n[DEBUG] Starting pipe execution\n", 2);

    if (pipe(pipefd) == -1)
    {
        ft_putstr_fd("[DEBUG] Pipe creation failed\n", 2);
        return (1);
    }

    ft_putstr_fd("[DEBUG] Pipe created: read=", 2);
    char fd_str[20];
    sprintf(fd_str, "%d", pipefd[0]);
    ft_putstr_fd(fd_str, 2);
    ft_putstr_fd(", write=", 2);
    sprintf(fd_str, "%d", pipefd[1]);
    ft_putstr_fd(fd_str, 2);
    ft_putstr_fd("\n", 2);

    pid1 = fork();
    if (pid1 < 0)
    {
        ft_putstr_fd("[DEBUG] First fork failed\n", 2);
        close(pipefd[0]);
        close(pipefd[1]);
        return (1);
    }

    if (pid1 == 0)
    {
        ft_putstr_fd("[DEBUG] In first child (echo)\n", 2);
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            ft_putstr_fd("[DEBUG] dup2 failed in first child\n", 2);
            exit(1);
        }
        close(pipefd[1]);
        
        // Добавим вывод аргументов команды
        if (node->left && node->left->args)
        {
            ft_putstr_fd("[DEBUG] Command args: ", 2);
            for (int i = 0; node->left->args[i]; i++)
            {
                ft_putstr_fd(node->left->args[i], 2);
                ft_putstr_fd(" ", 2);
            }
            ft_putstr_fd("\n", 2);
        }
        
        execute_command_in_pipe(node->left, env, fd_info);
        ft_putstr_fd("[DEBUG] First child execution failed\n", 2);
        exit(1);
    }

    pid2 = fork();
    if (pid2 < 0)
    {
        ft_putstr_fd("[DEBUG] Second fork failed\n", 2);
        close(pipefd[0]);
        close(pipefd[1]);
        kill(pid1, SIGTERM);
        waitpid(pid1, NULL, 0);
        return (1);
    }

    if (pid2 == 0)
    {
        ft_putstr_fd("[DEBUG] In second child (grep)\n", 2);
        close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            ft_putstr_fd("[DEBUG] dup2 failed in second child\n", 2);
            exit(1);
        }
        close(pipefd[0]);
        
        // Добавим вывод аргументов команды
        if (node->right && node->right->args)
        {
            ft_putstr_fd("[DEBUG] Command args: ", 2);
            for (int i = 0; node->right->args[i]; i++)
            {
                ft_putstr_fd(node->right->args[i], 2);
                ft_putstr_fd(" ", 2);
            }
            ft_putstr_fd("\n", 2);
        }
        
        if (node->right->type == AST_PIPE)
            execute_pipe_node(node->right, env, fd_info);
        else
            execute_command_in_pipe(node->right, env, fd_info);
        ft_putstr_fd("[DEBUG] Second child execution failed\n", 2);
        exit(1);
    }

    // Родительский процесс
    ft_putstr_fd("[DEBUG] In parent process\n", 2);
    close(pipefd[0]);
    close(pipefd[1]);

    // Ждем завершения обоих процессов
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status, 0);
    
    ft_putstr_fd("[DEBUG] Pipe execution completed\n", 2);
    return WEXITSTATUS(status);
}

int handle_pipe_redirect_out(t_redirect *redir)
{
    int fd;

    ft_putstr_fd("\n[DEBUG] Handling pipe redirect out for file: ", 2);
    ft_putstr_fd(redir->file, 2);
    ft_putstr_fd("\n", 2);

    if (redir->type == TOKEN_REDIR_OUT)
    {
        ft_putstr_fd("[DEBUG] Opening file in TRUNC mode\n", 2);
        fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }
    else if (redir->type == TOKEN_REDIR_APPEND)
    {
        ft_putstr_fd("[DEBUG] Opening file in APPEND mode\n", 2);
        fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    }
    else
        return 1;

    if (fd == -1)
    {
        ft_putstr_fd("[DEBUG] Failed to open file\n", 2);
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(redir->file, 2);
        ft_putstr_fd(": Permission denied\n", 2);
        return 0;
    }

    ft_putstr_fd("[DEBUG] File opened successfully, fd=", 2);
    char fd_str[20];
    sprintf(fd_str, "%d", fd);
    ft_putstr_fd(fd_str, 2);
    ft_putstr_fd("\n", 2);

    // Настраиваем редирект
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        ft_putstr_fd("[DEBUG] Failed to redirect stdout\n", 2);
        close(fd);
        return 0;
    }

    close(fd);
    return 1;
}

void execute_command_in_pipe(t_ast_node *cmd, t_env *env, t_fd_info *fd_info)
{
    ft_putstr_fd("[DEBUG] Entering execute_command_in_pipe\n", 2);

    if (!cmd || !cmd->args || !cmd->args[0])
    {
        ft_putstr_fd("[DEBUG] Invalid command\n", 2);
        exit(1);
    }

    ft_putstr_fd("[DEBUG] Executing command: ", 2);
    ft_putstr_fd(cmd->args[0], 2);
    ft_putstr_fd("\n", 2);

    // Обработка кавычек в аргументах
    for (int i = 0; cmd->args[i]; i++)
    {
        if (cmd->args[i][0] == '"' && cmd->args[i][strlen(cmd->args[i]) - 1] == '"')
        {
            // Удаляем кавычки
            char *temp = ft_strndup(cmd->args[i] + 1, strlen(cmd->args[i]) - 2);
            free(cmd->args[i]);
            cmd->args[i] = temp;
        }
    }

    if (cmd->redirects)
    {
        ft_putstr_fd("[DEBUG] Processing redirects\n", 2);
        if (!handle_command_redirections(cmd, fd_info))
        {
            ft_putstr_fd("[DEBUG] Redirect handling failed\n", 2);
            exit(1);
        }
    }

    if (is_builtin(cmd->args[0]))
    {
        ft_putstr_fd("[DEBUG] Executing builtin command\n", 2);
        int status;
        
        // Для builtin команд нужно убедиться, что вывод идет в пайп
        status = execute_builtin(cmd->args, env);
        
        // Важно: нужно сбросить буферы перед выходом
        fflush(stdout);
        exit(status);
    }

    ft_putstr_fd("[DEBUG] Executing external command\n", 2);
    setup_child_signals();
    execvp(cmd->args[0], cmd->args);
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(cmd->args[0], 2);
    ft_putstr_fd(": command not found\n", 2);
    exit(127);
}
