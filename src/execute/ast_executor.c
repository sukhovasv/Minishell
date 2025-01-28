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

/*int execute_command_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    if (!handle_command_redirections(node, fd_info, env))
		return 0;
        
    if (!node->args || !node->args[0])
        return 0;
        
    if (is_builtin(node->args[0]))
        return execute_builtin_command(node, env, fd_info);
    return execute_external_command(node, fd_info);
}*/

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

/*static void setup_pipe_child(int pipe_fd[2], int is_first)
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
}*/

static void close_pipe_fds(int pipe_fd[2])
{
    close(pipe_fd[0]);
    close(pipe_fd[1]);
}

/*static int wait_for_pipe_processes(pid_t pid1, pid_t pid2)
{
    int status2;

    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status2, 0);

    return ((status2 >> 8) & 0xff); // Возвращаем статус последнего процесса
}*/

/*int execute_pipe_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
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
}*/

/*int execute_pipe_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int pipe_fd[2];
    pid_t pid1, pid2;
    int status;

    if (pipe(pipe_fd) == -1)
        return 1;

    // Первый процесс (левая часть пайпа)
    pid1 = fork();
    if (pid1 == -1)
    {
        close_pipe_fds(pipe_fd);
        return 1;
    }
    if (pid1 == 0)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        exit(execute_ast_node(node->left, env, fd_info));
    }

    // Второй процесс (правая часть пайпа)
    pid2 = fork();
    if (pid2 == -1)
    {
        close_pipe_fds(pipe_fd);
        kill(pid1, SIGTERM);
        waitpid(pid1, NULL, 0);
        return 1;
    }
    if (pid2 == 0)
    {
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);
        exit(execute_ast_node(node->right, env, fd_info));
    }

    // Родительский процесс
    close_pipe_fds(pipe_fd);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status, 0);
    return WEXITSTATUS(status);
}*/

int execute_pipe_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
   int pipe_fd[2];
   pid_t pid1, pid2;
   int status;
   int heredoc_stdin = -1;
   int original_stdin = dup(STDIN_FILENO);

   if (original_stdin == -1)
   {
       perror("Failed to save original stdin");
       return 1;
   }

   if (pipe(pipe_fd) == -1)
   {
       perror("pipe failed");
       close(original_stdin);
       return 1;
   }

   // Обрабатываем heredoc перед fork()
   if (node->left && node->left->redirects)
   {
       t_redirect *redir = node->left->redirects;
       while (redir)
       {
           if (redir->type == TOKEN_REDIR_HEREDOC)
           {
               if (!handle_redir_heredoc(redir, fd_info, env, node->left))
               {
                   close_pipe_fds(pipe_fd);
                   close(original_stdin);
                   return 1;
               }
               heredoc_stdin = dup(STDIN_FILENO);
               printf("DEBUG: Saved heredoc content to fd %d\n", heredoc_stdin);
               if (heredoc_stdin == -1)
               {
                   close_pipe_fds(pipe_fd);
                   close(original_stdin);
                   return 1;
               }
               break;
           }
           redir = redir->next;
       }
   }

   if (dup2(original_stdin, STDIN_FILENO) == -1)
   {
       close_pipe_fds(pipe_fd);
       if (heredoc_stdin != -1)
           close(heredoc_stdin);
       close(original_stdin);
       return 1;
   }
   close(original_stdin);

   pid1 = fork();
   if (pid1 == -1)
   {
       perror("First fork failed");
       if (heredoc_stdin != -1)
           close(heredoc_stdin);
       close_pipe_fds(pipe_fd);
       return 1;
   }

   if (pid1 == 0)  // Первый дочерний процесс
   {
       close(pipe_fd[0]);

       if (heredoc_stdin != -1)
       {
           printf("DEBUG: Child 1: Restoring heredoc content from fd %d\n", heredoc_stdin);
           if (dup2(heredoc_stdin, STDIN_FILENO) == -1)
           {
               perror("Failed to redirect heredoc stdin");
               close(heredoc_stdin);
               exit(1);
           }
           close(heredoc_stdin);
           
           // Читаем и передаем данные через pipe
           char buf[1024];
           ssize_t n;
           while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0)
           {
               if (write(pipe_fd[1], buf, n) != n)
               {
                   perror("Write to pipe failed");
                   exit(1);
               }
           }
       }

       close(pipe_fd[1]);
       printf("DEBUG: Child 1: Execution complete\n");
       exit(0);
   }

   // Родительский процесс ждет завершения записи первого процесса
   close(pipe_fd[1]);
   if (heredoc_stdin != -1)
       close(heredoc_stdin);

   pid2 = fork();
   if (pid2 == -1)
   {
       perror("Second fork failed");
       close(pipe_fd[0]);
       kill(pid1, SIGTERM);
       waitpid(pid1, NULL, 0);
       return 1;
   }

   if (pid2 == 0)  // Второй дочерний процесс
   {
       printf("DEBUG: Child 2: Setting up pipe input\n");
       
       if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
       {
           perror("Failed to redirect pipe to stdin");
           exit(1);
       }
       close(pipe_fd[0]);

       printf("DEBUG: Child 2: Executing wc\n");
       exit(execute_ast_node(node->right, env, fd_info));
   }

   // Родительский процесс
   close(pipe_fd[0]);

   // Ждем завершения обоих процессов
   waitpid(pid1, &status, 0);
   printf("DEBUG: Child 1 (cat) exited with status %d\n", WIFEXITED(status) ? WEXITSTATUS(status) : status);
   
   int status2;
   waitpid(pid2, &status2, 0);
   printf("DEBUG: Child 2 (wc) exited with status %d\n", WIFEXITED(status2) ? WEXITSTATUS(status2) : status2);

   return WIFEXITED(status2) ? WEXITSTATUS(status2) : status2;
}


/*int execute_ast_redirect_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    if (node->redirects)
    {
        if (!handle_redirections(node->redirects, fd_info, env, node))
            return 1;
    }
    return execute_ast_node(node->left, env, fd_info);
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
