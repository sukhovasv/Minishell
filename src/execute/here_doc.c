#include "minishell.h"

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

    ignore_signals();
    status = fork_and_process_heredocs(heredocs, count, env);
    setup_signals();  // Полностью восстанавливаем состояние терминала

    return (status != -1 && WIFEXITED(status) && WEXITSTATUS(status) == 0);
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
}
