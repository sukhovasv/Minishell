#include "minishell.h"

static int handle_single_heredoc(t_redirect *redir, t_fd_info *fd_info)
{
    int fd;

    if (!redir->token || !redir->token->temp_file)
        return 0;

    fd = open(redir->token->temp_file, O_RDONLY);
    if (fd == -1)
        return 0;

    if (fd_info->saved_stdin == -1)
        fd_info->saved_stdin = dup(STDIN_FILENO);

    dup2(fd, STDIN_FILENO);
    close(fd);

    return 1;
}

static int handle_command_redirections(t_ast_node *node, t_fd_info *fd_info)
{
    t_redirect *redir;

    if (!node->redirects)
        return 1;

    redir = node->redirects;
    while (redir)
    {
        if (redir->type == TOKEN_REDIR_HEREDOC)
        {
            if (!handle_single_heredoc(redir, fd_info))
                return 0;
        }
        else if (redir->type == TOKEN_REDIR_IN)
        {
            if (!handle_redir_input(redir, &fd_info->saved_stdin))
                return 0;
        }
        else if (redir->type == TOKEN_REDIR_OUT)
        {
            if (!handle_redir_output(redir, &fd_info->saved_stdout))
                return 0;
        }
        else if (redir->type == TOKEN_REDIR_APPEND)
        {
            if (!handle_redir_append(redir, &fd_info->saved_stdout))
                return 0;
        }
        redir = redir->next;
    }
    return 1;
}

static int execute_builtin_command(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int status;

    if (!handle_command_redirections(node, fd_info))
        return 1;

    status = execute_builtin(node->args, env);
    restore_redirections(fd_info);
    return status;
}

static int execute_external_command(t_ast_node *node, t_fd_info *fd_info)
{
    pid_t pid;
    int status;

    if (!handle_command_redirections(node, fd_info))
        return 1;

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
    return WEXITSTATUS(status);
}

int execute_command_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int status;

    if (!node->args || !node->args[0])
        return 0;

    if (is_builtin(node->args[0]))
        status = execute_builtin_command(node, env, fd_info);
    else
        status = execute_external_command(node, fd_info);

    return status;
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

static void execute_command_in_pipe(t_ast_node *cmd, t_env *env, t_fd_info *fd_info)
{
    ft_putstr_fd("\n[DEBUG] Entering execute_command_in_pipe\n", 2);

    if (!cmd || !cmd->args || !cmd->args[0])
    {
        ft_putstr_fd("[DEBUG] Invalid command\n", 2);
        _exit(1);
    }

    ft_putstr_fd("[DEBUG] Executing command: ", 2);
    ft_putstr_fd(cmd->args[0], 2);
    ft_putstr_fd("\n", 2);

    // Обрабатываем все редиректы
    if (cmd->redirects)
    {
        ft_putstr_fd("[DEBUG] Processing redirects\n", 2);
        t_redirect *redir = cmd->redirects;
        
        // Обрабатываем heredoc и input редиректы
        while (redir)
        {
            if (redir->type == TOKEN_REDIR_HEREDOC || redir->type == TOKEN_REDIR_IN)
            {
                ft_putstr_fd("[DEBUG] Processing heredoc/input redirect\n", 2);
                if (!handle_command_redirections(cmd, fd_info))
                {
                    ft_putstr_fd("[DEBUG] Failed to handle heredoc/input redirect\n", 2);
                    _exit(1);
                }
                break;
            }
            redir = redir->next;
        }

        // Затем обрабатываем редиректы вывода
        redir = cmd->redirects;
        while (redir)
        {
            if (redir->type == TOKEN_REDIR_OUT || redir->type == TOKEN_REDIR_APPEND)
            {
                ft_putstr_fd("[DEBUG] Processing output redirect\n", 2);
                if (!handle_pipe_redirect_out(redir))
                {
                    ft_putstr_fd("[DEBUG] Failed to handle output redirect\n", 2);
                    _exit(1);
                }
            }
            redir = redir->next;
        }
    }

    if (is_builtin(cmd->args[0]))
    {
        ft_putstr_fd("[DEBUG] Executing builtin command\n", 2);
        int status = execute_builtin(cmd->args, env);
        ft_putstr_fd("[DEBUG] Builtin execution completed with status: ", 2);
        char status_str[10];
        sprintf(status_str, "%d", status);
        ft_putstr_fd(status_str, 2);
        ft_putstr_fd("\n", 2);
        _exit(status);
    }

    ft_putstr_fd("[DEBUG] Executing external command\n", 2);
    setup_child_signals();
    execvp(cmd->args[0], cmd->args);
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(cmd->args[0], 2);
    ft_putstr_fd(": command not found\n", 2);
    _exit(127);
}

int execute_pipe_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int pipefd[2];
    pid_t pid1, pid2;
    int status;

    ft_putstr_fd("\n[DEBUG] Entering execute_pipe_node\n", 2);

    if (pipe(pipefd) == -1)
    {
        ft_putstr_fd("[DEBUG] Failed to create pipe\n", 2);
        return (1);
    }

    ft_putstr_fd("[DEBUG] Created pipe: read=", 2);
    char fd_str[20];
    sprintf(fd_str, "%d", pipefd[0]);
    ft_putstr_fd(fd_str, 2);
    ft_putstr_fd(", write=", 2);
    sprintf(fd_str, "%d", pipefd[1]);
    ft_putstr_fd(fd_str, 2);
    ft_putstr_fd("\n", 2);

    // Сначала создаем оба процесса
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
        ft_putstr_fd("[DEBUG] In first child process\n", 2);
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execute_command_in_pipe(node->left, env, fd_info);
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
        ft_putstr_fd("[DEBUG] In second child process\n", 2);
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        if (node->right->type == AST_PIPE)
            execute_pipe_node(node->right, env, fd_info);
        else
            execute_command_in_pipe(node->right, env, fd_info);
    }

    // Родительский процесс закрывает оба конца пайпа
    close(pipefd[0]);
    close(pipefd[1]);
    
    // Ждем завершения обоих процессов
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status, 0);
    
    ft_putstr_fd("[DEBUG] Pipe execution completed\n", 2);
    return WEXITSTATUS(status);
}

int execute_ast_redirect_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int status;

    if (!handle_command_redirections(node, fd_info))
        return 1;

    status = execute_ast_node(node->left, env, fd_info);
    restore_redirections(fd_info);
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
        
    return 1;
}
