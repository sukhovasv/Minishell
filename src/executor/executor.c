#include "minishell.h"

t_ast_node	*prepare_command(char *input, t_env *env)
{
	t_token		*tokens;
	char		*expanded;
	t_ast_node	*cmd;

	expanded = parse_quotes(input, env);
	if (!expanded)
		return (NULL);
	tokens = tokenize(expanded, env);
	free(expanded);
	if (!tokens)
		return (NULL);
	cmd = build_ast(tokens);
	free_tokens(tokens);
	return (cmd);
}

int	execute_command(char *input, t_env *env)
{
	t_ast_node	*cmd;
	int			status;
	t_fd_info	fd_info;

	fd_info.saved_stdout = -1;
	fd_info.saved_stdin = -1;
	cmd = prepare_command(input, env);
	if (!cmd)
	{
		if (!input || !*input)
			return (0);
		return (1);
	}
	status = execute_ast_node(cmd, env, &fd_info);
	restore_redirections(&fd_info);
	free_ast_node(cmd);
	return (status);
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

int handle_single_heredoc(t_redirect *redir, t_fd_info *fd_info)
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

int handle_command_redirections(t_ast_node *node, t_fd_info *fd_info)
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


int execute_builtin_command(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
    int status;

    if (!handle_command_redirections(node, fd_info))
        return 1;

    status = execute_builtin(node->args, env);
    restore_redirections(fd_info);
    return status;
}

