#include "minishell.h"

bool handle_redir(char tok, char **str, t_token **tokens)
{
	if (*(*str + 1) == tok)
	{
		if (tok == '<')
			append_token(tokens, init_token("<<", REDIR_HEREDOC));
		else if (tok == '>')
			append_token(tokens, init_token(">>", REDIR_APPEND));
		(*str)++;
		return (true);
	}
	return (false);
}


char	*tokenise_special_chars(char *str, t_token **tokens)
{
	if (*str == '<')
	{
		if (!handle_redir(*str, &str, tokens))
			append_token(tokens, init_token("<", REDIR_IN));
	}
	else if (*str == '>')
	{
		if (!handle_redir(*str, &str, tokens))
			append_token(tokens, init_token(">", REDIR_OUT));
	}
	else if (*str == '|')
		append_token(tokens, init_token("|", PIPE));
	(str)++;
	return (str);
}
