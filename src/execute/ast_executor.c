#include "minishell.h"

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
    if (!handle_command_redirections(node, fd_info, env))
		return 0;
        
    if (!node->args || !node->args[0])
        return 0;
        
    if (is_builtin(node->args[0]))
        return execute_builtin_command(node, env, fd_info);
    return execute_external_command(node, fd_info);
}

static void setup_pipe_child(int pipe_fd[2], int is_first)
{
    if (is_first)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }
    else
    {
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);
    }
}

static int setup_pipe_and_fork(int pipe_fd[2])
{
    if (pipe(pipe_fd) == -1)
        return 0;
    return 1;
}

static void close_pipe_fds(int pipe_fd[2])
{
    close(pipe_fd[0]);
    close(pipe_fd[1]);
}

static int wait_for_pipe_processes(pid_t pid1, pid_t pid2)
{
    int status2;

    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status2, 0);

    return ((status2 >> 8) & 0xff); // Возвращаем статус последнего процесса
}

int execute_pipe_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int pipe_fd[2];

    if (!setup_pipe_and_fork(pipe_fd))
        return 1;
    pid_t pid1 = fork();
    if (pid1 == 0)
    {
        setup_pipe_child(pipe_fd, 1);
        int status1 = execute_ast_node(node->left, env, fd_info);
        exit(status1);
    }
    pid_t pid2 = fork();
    if (pid2 == 0)
    {
        setup_pipe_child(pipe_fd, 0);
        int status2 = execute_ast_node(node->right, env, fd_info);
        exit(status2);
    }
    close_pipe_fds(pipe_fd);
    return wait_for_pipe_processes(pid1, pid2);
}

int execute_ast_redirect_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    if (node->redirects)
    {
        if (!handle_redirections(node->redirects, fd_info, env, node))
            return 1;
    }
    return execute_ast_node(node->left, env, fd_info);
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
