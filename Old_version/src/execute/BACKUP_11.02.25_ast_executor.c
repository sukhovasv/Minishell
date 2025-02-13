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

/*static int handle_command_redirections(t_ast_node *node, t_fd_info *fd_info)
{
    t_redirect *redir;

    if (!node->redirects)
        return 1;

    fd_info->saved_stdout = -1;
    fd_info->saved_stdin = -1;

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
}*/

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

/*static void execute_command_in_pipe(t_ast_node *cmd, t_env *env, t_fd_info *fd_info)
{
    if (!cmd || !cmd->args || !cmd->args[0])
        exit(1);

    if (cmd->redirects && !handle_command_redirections(cmd, fd_info))
        exit(1);

    if (is_builtin(cmd->args[0]))
    {
        int status = execute_builtin(cmd->args, env);
        exit(status);
    }

    setup_child_signals();
    execvp(cmd->args[0], cmd->args);
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(cmd->args[0], 2);
    ft_putstr_fd(": command not found\n", 2);
    exit(127);
}*/

static void setup_pipe_child(int read_pipe, int write_pipe)
{
    if (read_pipe != STDIN_FILENO)
    {
        dup2(read_pipe, STDIN_FILENO);
        close(read_pipe);
    }
    if (write_pipe != STDOUT_FILENO)
    {
        dup2(write_pipe, STDOUT_FILENO);
        close(write_pipe);
    }
}

/*int execute_pipe_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int pipefd[2];
    pid_t pid1, pid2;
    int status;

    if (pipe(pipefd) == -1)
        return (1);

    pid1 = fork();
    if (pid1 < 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (1);
    }

    if (pid1 == 0)
    {
        close(pipefd[0]);
        setup_pipe_child(STDIN_FILENO, pipefd[1]);
        execute_command_in_pipe(node->left, env, fd_info);
    }

    pid2 = fork();
    if (pid2 < 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (1);
    }

    if (pid2 == 0)
    {
        close(pipefd[1]);
        setup_pipe_child(pipefd[0], STDOUT_FILENO);
        if (node->right->type == AST_PIPE)
            execute_pipe_node(node->right, env, fd_info);
        else
            execute_command_in_pipe(node->right, env, fd_info);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status, 0);
    
    return WEXITSTATUS(status);
}*/

/*static void execute_command_in_pipe(t_ast_node *cmd, t_env *env, t_fd_info *fd_info)
{
    if (!cmd || !cmd->args || !cmd->args[0])
        _exit(1);  // Используем _exit вместо exit

    if (cmd->redirects && !handle_command_redirections(cmd, fd_info))
        _exit(1);

    if (is_builtin(cmd->args[0]))
    {
        int status = execute_builtin(cmd->args, env);
        _exit(status);  // Используем _exit вместо exit
    }

    setup_child_signals();
    execvp(cmd->args[0], cmd->args);
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(cmd->args[0], 2);
    ft_putstr_fd(": command not found\n", 2);
    _exit(127);  // Используем _exit вместо exit
}*/

/*static void execute_command_in_pipe(t_ast_node *cmd, t_env *env)
{
    t_fd_info local_fd_info = {-1, -1};  // Локальная копия для каждого процесса

    if (!cmd || !cmd->args || !cmd->args[0])
        _exit(1);

    // Обрабатываем редиректы до установки пайпов
    if (cmd->redirects && !handle_command_redirections(cmd, &local_fd_info))
        _exit(1);

    if (is_builtin(cmd->args[0]))
    {
        int status = execute_builtin(cmd->args, env);
        restore_redirections(&local_fd_info);  // Восстанавливаем редиректы
        _exit(status);
    }

    setup_child_signals();
    execvp(cmd->args[0], cmd->args);
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(cmd->args[0], 2);
    ft_putstr_fd(": command not found\n", 2);
    restore_redirections(&local_fd_info);  // Восстанавливаем редиректы
    _exit(127);
}*/

/*static void execute_command_in_pipe(t_ast_node *cmd, t_env *env, t_fd_info *fd_info)
{
    if (!cmd || !cmd->args || !cmd->args[0])
        _exit(1);

    if (cmd->redirects && !handle_command_redirections(cmd, fd_info))
        _exit(1);

    if (is_builtin(cmd->args[0]))
    {
        int status = execute_builtin(cmd->args, env);
        _exit(status);
    }

    setup_child_signals();
    execvp(cmd->args[0], cmd->args);
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(cmd->args[0], 2);
    ft_putstr_fd(": command not found\n", 2);
    _exit(127);
}*/

static int handle_output_redirect(t_ast_node *cmd)
{
    t_redirect *redir;
    int fd;

    if (!cmd || !cmd->redirects)
        return 1;

    redir = cmd->redirects;
    while (redir)
    {
        if (redir->type == TOKEN_REDIR_OUT)
        {
            fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
                return 0;
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir->type == TOKEN_REDIR_APPEND)
        {
            fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
                return 0;
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        redir = redir->next;
    }
    return 1;
}

static void execute_command_in_pipe(t_ast_node *cmd, t_env *env, t_fd_info *fd_info)
{
    if (!cmd || !cmd->args || !cmd->args[0])
        _exit(1);

    // Обрабатываем редиректы heredoc как обычно
    if (cmd->redirects)
    {
        t_redirect *redir = cmd->redirects;
        while (redir)
        {
            if (redir->type == TOKEN_REDIR_HEREDOC)
            {
                if (!handle_command_redirections(cmd, fd_info))
                    _exit(1);
                break;
            }
            redir = redir->next;
        }
    }

    // Отдельно обрабатываем редиректы вывода
    if (!handle_output_redirect(cmd))
        _exit(1);

    if (is_builtin(cmd->args[0]))
    {
        int status = execute_builtin(cmd->args, env);
        _exit(status);
    }

    setup_child_signals();
    execvp(cmd->args[0], cmd->args);
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(cmd->args[0], 2);
    ft_putstr_fd(": command not found\n", 2);
    _exit(127);
}

/*int execute_pipe_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int pipefd[2];
    pid_t pid1, pid2;
    int status;

    if (pipe(pipefd) == -1)
        return (1);

    pid1 = fork();
    if (pid1 < 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (1);
    }

    if (pid1 == 0)
    {
        close(pipefd[0]);
        setup_pipe_child(STDIN_FILENO, pipefd[1]);
        execute_command_in_pipe(node->left, env, fd_info);
        _exit(1);  // На случай, если execute_command_in_pipe не завершится
    }

    pid2 = fork();
    if (pid2 < 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (1);
    }

    if (pid2 == 0)
    {
        close(pipefd[1]);
        setup_pipe_child(pipefd[0], STDOUT_FILENO);
        if (node->right->type == AST_PIPE)
            execute_pipe_node(node->right, env, fd_info);
        else
            execute_command_in_pipe(node->right, env, fd_info);
        _exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status, 0);
    
    return WEXITSTATUS(status);
}*/

/*static int setup_output_redirect(t_ast_node *node)
{
    t_redirect *redir;
    int original_stdout = -1;
    int fd;

    if (!node || !node->redirects)
        return -1;

    redir = node->redirects;
    while (redir)
    {
        if (redir->type == TOKEN_REDIR_OUT || redir->type == TOKEN_REDIR_APPEND)
        {
            if (original_stdout == -1)
                original_stdout = dup(STDOUT_FILENO);

            fd = (redir->type == TOKEN_REDIR_OUT) ?
                open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644) :
                open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);

            if (fd == -1)
            {
                if (original_stdout != -1)
                {
                    dup2(original_stdout, STDOUT_FILENO);
                    close(original_stdout);
                }
                return -1;
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        redir = redir->next;
    }
    return original_stdout;
}*/

int execute_pipe_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int pipefd[2];
    pid_t pid1, pid2;
    int status;

    if (pipe(pipefd) == -1)
        return (1);

    pid1 = fork();
    if (pid1 < 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (1);
    }

    if (pid1 == 0)
    {
        close(pipefd[0]);
        setup_pipe_child(STDIN_FILENO, pipefd[1]);
        execute_command_in_pipe(node->left, env, fd_info);
        _exit(1);
    }

    pid2 = fork();
    if (pid2 < 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        kill(pid1, SIGTERM);
        waitpid(pid1, NULL, 0);
        return (1);
    }

    if (pid2 == 0)
    {
        close(pipefd[1]);
        setup_pipe_child(pipefd[0], STDOUT_FILENO);
        if (node->right->type == AST_PIPE)
            execute_pipe_node(node->right, env, fd_info);
        else
            execute_command_in_pipe(node->right, env, fd_info);
        _exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status, 0);
    
    return WEXITSTATUS(status);
}

/*int execute_pipe_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int pipefd[2];
    pid_t pid1, pid2;
    int status;

    if (pipe(pipefd) == -1)
        return (1);

    pid1 = fork();
    if (pid1 < 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (1);
    }

    if (pid1 == 0)
    {
        close(pipefd[0]);
        setup_pipe_child(STDIN_FILENO, pipefd[1]);
        execute_command_in_pipe(node->left, env);  // Убран fd_info
    }

    pid2 = fork();
    if (pid2 < 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        kill(pid1, SIGTERM);
        waitpid(pid1, NULL, 0);
        return (1);
    }

    if (pid2 == 0)
    {
        close(pipefd[1]);
        setup_pipe_child(pipefd[0], STDOUT_FILENO);
        if (node->right->type == AST_PIPE)
            execute_pipe_node(node->right, env, fd_info);
        else
            execute_command_in_pipe(node->right, env);  // Убран fd_info
    }

    close(pipefd[0]);
    close(pipefd[1]);
    
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status, 0);
    
    return WEXITSTATUS(status);
}*/

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
