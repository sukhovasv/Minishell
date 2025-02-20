/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command_executor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:22:58 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:23:00 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static void	command_not_found_error(char *cmd, t_env *env)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	builtin_exit_wrapper(env, 127);
}

static void	validate_command(t_ast_node *cmd, t_env *env)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		builtin_exit_wrapper(env, 1);
	handle_quotes_in_args(cmd->args);
}

void	execute_command_in_pipe(t_ast_node *cmd, t_env *env, t_fd_info *fd_info)
{
	int		status;
	char	*path;
	char	fullpath[PATH_MAX];

	validate_command(cmd, env);
	if (cmd->redirects && !handle_command_redirections(cmd, fd_info))
		builtin_exit_wrapper(env, 1);
	if (is_builtin(cmd->args[0]))
	{
		status = execute_builtin_wrapper(cmd->args, env);
		builtin_exit_wrapper(env, status);
	}
	setup_child_signals();
	if (ft_strchr(cmd->args[0], '/'))
		execve(cmd->args[0], cmd->args, env->environ);
	else
	{
		path = get_env_value("PATH", env);
		status = try_path_execution(path, cmd->args, fullpath);
		free(path);
		if (status != -1)
			execve(fullpath, cmd->args, env->environ);
	}
	command_not_found_error(cmd->args[0], env);
}
