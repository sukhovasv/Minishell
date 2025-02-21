/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:27:16 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:27:18 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

static void	init_shell(t_env **env, t_fd_info *fd_info, int *is_interactive)
{
	extern char	**environ;

	*env = init_env(environ);
	setup_signals(*env);
	*is_interactive = isatty(STDIN_FILENO) && isatty(STDERR_FILENO);
	fd_info->saved_stdout = -1;
	fd_info->saved_stdin = -1;
}

static char	*get_input_prompt(int is_interactive)
{
	char	*line;
	char	buf[4096];
	ssize_t	bytes_read;

	rl_event_hook = handle_pending_signals;
	line = NULL;
	if (!is_interactive)
	{
		bytes_read = read(STDIN_FILENO, buf, sizeof(buf) - 1);
		if (bytes_read > 0)
		{
			buf[bytes_read] = '\0';
			if (bytes_read > 0 && buf[bytes_read - 1] == '\n')
				buf[bytes_read - 1] = '\0';
			line = ft_strdup(buf);
		}
		return (line);
	}
	return (readline("minishell> "));
}

static void	process_command(char *input, t_env *env, t_fd_info *fd_info,
		int is_interactive)
{
	t_token		*tokens;
	t_ast_node	*ast;
	int			status;

	if (!input || !*input)
		return ;
	if (is_interactive)
		add_history(input);
	tokens = tokenize(input, env);
	if (tokens)
	{
		ast = build_ast(tokens);
		if (!ast)
		{
			free_tokens(tokens);
			return ;
		}
		env->ast = ast;
		env->tokens = tokens;
		status = execute_ast_node(ast, env, fd_info);
		env->tokens = (free_tokens(tokens), NULL);
		env->ast = (free_ast_node(ast), NULL);
		env->last_status = status;
	}
}

static void	shell_loop(t_env *env, t_fd_info *fd_info, int is_interactive)
{
	char	*input;

	while (1)
	{
		input = get_input_prompt(is_interactive);
		if (!input)
		{
			printf("\n");
			break ;
		}
		if (g_signal_received == SIGINT)
		{
			g_signal_received = 0;
			free(input);
			continue ;
		}
		if (*input)
		{
			process_command(input, env, fd_info, is_interactive);
			rl_already_prompted = 0;
		}
		free(input);
	}
}

int	main(void)
{
	t_minishell_data	data;
	t_fd_info			fd_info;
	int					is_interactive;
	struct termios		original;

	ft_memset(&data, 0, sizeof(t_minishell_data));
	tcgetattr(STDIN_FILENO, &original);
	init_shell(&data.env, &fd_info, &is_interactive);
	if (!data.env)
		return (EXIT_FAILURE);
	shell_loop(data.env, &fd_info, is_interactive);
	tcsetattr(STDIN_FILENO, TCSANOW, &original);
	rl_clear_history();
	clear_history();
	free_all(&data);
	return (0);
}
