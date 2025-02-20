/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                  			:+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:19:07 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:19:11 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# define _GNU_SOURCE

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <errno.h>
# include <limits.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

extern volatile sig_atomic_t	g_signal_received;

typedef struct s_parser_state
{
	int		in_quotes;
	char	quote_char;
}	t_parser_state;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	int				has_quotes;
	char			*heredoc_content;
	char			*temp_file;
}	t_token;

typedef struct s_redirect
{
	int					type;
	char				*file;
	struct s_redirect	*next;
	t_token				*token;
}	t_redirect;

typedef struct s_heredoc_data
{
	char	**lines;
	int		count;
	int		capacity;
	char	*content;
	char	*temp_file;
	int		temp_fd;
	char	*delimiter;
	int		expand_vars;
	t_token	*token;
}	t_heredoc_data;

typedef struct s_fd_info
{
	int	saved_stdout;
	int	saved_stdin;
}	t_fd_info;

typedef struct s_quote_state
{
	int		in_quotes;
	char	quote_type;
	char	*result;
	size_t	capacity;
	size_t	length;
	size_t	i;
	size_t	j;
	size_t	len;
}	t_quote_state;

typedef enum e_ast_type
{
	AST_COMMAND,
	AST_PIPE,
	AST_REDIRECT,
}	t_ast_type;

typedef struct s_ast_node
{
	t_ast_type			type;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	char				**args;
	t_redirect			*redirects;
	int					pipe_level;
	t_heredoc_data		*heredoc_data;
	int					heredoc_count;
}	t_ast_node;

typedef struct s_env
{
	char				**environ;
	int					last_status;
	t_ast_node			*ast;
	t_token				*tokens;
	struct sigaction	old_sigactions[5];
	struct sigaction	new_sigactions[5];
}	t_env;

typedef struct s_word_parser
{
	const char	*str;
	char		*result;
	size_t		i;
	size_t		j;
	size_t		len;
	int			in_quotes;
	char		quote_char;
	t_env		*env;
}	t_word_parser;

typedef struct s_str_processor
{
	char	*result;
	int		*i;
	int		*j;
}	t_str_processor;

typedef struct s_minishell_data
{
	t_env			*env;
	t_token			*tokens;
	t_redirect		*redirects;
	t_heredoc_data	*heredoc;
	char			*input;
}	t_minishell_data;

#endif
