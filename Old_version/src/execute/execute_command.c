/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:53:05 by ssukhova          #+#    #+#             */
/*   Updated: 2024/07/16 12:53:10 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ast_node	*prepare_command(char *input, t_env *env)
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
