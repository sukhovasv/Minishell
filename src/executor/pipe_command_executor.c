#include "minishell.h"

static void	handle_quotes_in_args(char **args)
{
	int		i;
	char	*temp;

	i = 0;
	while (args[i])
	{
		if (args[i][0] == '"' && args[i][ft_strlen(args[i]) - 1] == '"')
		{
			temp = ft_strndup(args[i] + 1, ft_strlen(args[i]) - 2);
			free(args[i]);
			args[i] = temp;
		}
		i++;
	}
}

static void	command_not_found_error(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(127);
}

void	execute_command_in_pipe(t_ast_node *cmd, t_env *env, t_fd_info *fd_info)
{
	int	status;

	if (!cmd || !cmd->args || !cmd->args[0])
		exit(1);
	handle_quotes_in_args(cmd->args);
	if (cmd->redirects)
	{
		if (!handle_command_redirections(cmd, fd_info))
			exit(1);
	}
	if (is_builtin(cmd->args[0]))
	{
		status = execute_builtin(cmd->args, env);
		exit(status);
	}
	setup_child_signals();
	execvp(cmd->args[0], cmd->args);
	command_not_found_error(cmd->args[0]);
}
