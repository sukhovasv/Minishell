#include "minishell.h"

int	execute_ast_redirect_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
	int	status;

	if (!handle_command_redirections(node, fd_info))
		return (1);
	status = execute_ast_node(node->left, env, fd_info);
	restore_redirections(fd_info);
	return (status);
}

int	execute_ast_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
	if (!node)
		return (1);
	if (node->type == AST_COMMAND)
		return (execute_command_node(node, env, fd_info));
	else if (node->type == AST_PIPE)
		return (execute_pipe_node(node, env, fd_info));
	else if (node->type == AST_REDIRECT)
		return (execute_ast_redirect_node(node, env, fd_info));
	return (1);
}

int	execute_command_node(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
	int	status;

	if (!node->args || !node->args[0])
		return (0);
	if (is_builtin(node->args[0]))
		status = execute_builtin_command(node, env, fd_info);
	else
		status = execute_external_command(node, fd_info, env);
	return (status);
}

int	execute_builtin_command(t_ast_node *node, t_env *env, t_fd_info *fd_info)
{
	int	status;

	if (!handle_command_redirections(node, fd_info))
		return (1);
	//status = execute_builtin(node->args, env);
	status = execute_builtin_wrapper(node->args, env);
	restore_redirections(fd_info);
	return (status);
}
