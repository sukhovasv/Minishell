/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:22:35 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:22:37 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sysexits.h>
#include "minishell.h"

int	execute_external_command(t_ast_node *node, t_fd_info *fd_info, t_env *env)
{
	pid_t	pid;
	int		status;
	int		wstatus;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		if (!handle_command_redirections(node, fd_info))
			builtin_exit_wrapper(env, EXIT_FAILURE);
		reset_sighandlers(env);
		status = search_and_execute(node->args, env);
		builtin_exit_wrapper(env, status);
	}
	else if (pid < 0)
		return (perror("minishell: fork"), EXIT_FAILURE);
	else
		status = (ft_decode_wstatus(ft_wait_for_pid(&wstatus, pid)));
	sigaction(SIGINT, &env->new_sigactions[0], NULL);
	restore_redirections(fd_info);
	return (status);
}

int	search_and_execute(char **argv, t_env *env)
{
	int			status;
	char		*path;
	char *const	fullpath = (char [PATH_MAX]){0};

	errno = ENOENT;
	status = try_direct_execution(argv, fullpath);
	if ((status == -1) && (errno == ENOENT || errno == EACCES))
	{
		path = get_env_value("PATH", env);
		status = try_path_execution(path, argv, fullpath);
		free(path);
	}
	if (status == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(argv[0]);
	}
	else
		status = try_execute(fullpath, argv, env);
	return (status);
}

int	try_path_execution(char *path, char **argv, char *const fullpath)
{
	char	**paths;
	int		i;
	int		errcode;

	paths = ft_split(path, ':');
	if (!paths)
		return (EXIT_FAILURE);
	i = -1;
	errcode = -1;
	errno = ENOENT;
	while (((errcode == -1) && (errno == ENOENT || errno == EACCES))
		&& paths[++i])
	{
		ft_strlcpy(fullpath, paths[i], PATH_MAX);
		ft_strlcat(fullpath, "/", PATH_MAX);
		ft_strlcat(fullpath, argv[0], PATH_MAX);
		errcode = access(fullpath, F_OK);
	}
	free_args_array(paths);
	if ((errcode == -1) && (errno == ENOENT || errno == EACCES))
		errcode = access(argv[0], F_OK);
	else
		errcode = EX_NOTFOUND;
	return (errcode);
}

int	try_execute(char *cmd, char **argv, t_env *env)
{
	int	status;

	status = EX_OK;
	if (access(cmd, F_OK) == -1)
		status = EX_NOTFOUND;
	else
	{
		status = execve(cmd, argv, env->environ);
		if (errno == EACCES)
			status = EX_NOEXEC;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": Permission denied", 2);
	}
	return (status);
}
