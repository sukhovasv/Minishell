#include "minishell.h"

/*void free_env(t_env *env)
{
	int i;

	if (!env)
		return;
	if (env->environ)
	{
		i = 0;
		while (env->environ[i])
			free(env->environ[i++]);
		free(env->environ);
	}
	free(env);
}

void free_tokens(t_token *tokens)
{
	t_token *next;

	while (tokens)
	{
		next = tokens->next;
		free(tokens->value);
		free(tokens->heredoc_content);
		free(tokens->temp_file);
		free(tokens);
		tokens = next;
	}
}

void free_redirections(t_redirect *redirects)
{
	t_redirect *next;

	while (redirects)
	{
		next = redirects->next;
		free(redirects->file);
		free_tokens(redirects->token); // Освобождаем связанный токен
		free(redirects);
		redirects = next;
	}
}

void free_heredoc_data(t_heredoc_data *heredoc)
{
	int i;

	if (!heredoc)
		return;
	if (heredoc->lines)
	{
		for (i = 0; i < heredoc->count; i++)
			free(heredoc->lines[i]);
		free(heredoc->lines);
	}
	free(heredoc->content);
	free(heredoc->temp_file);
	free(heredoc->delimiter);
	free_tokens(heredoc->token);
	free(heredoc);
}

void free_ast_node(t_ast_node *node)
{
	if (!node)
		return;
	if (node->args)
	{
		int i = 0;
		while (node->args[i])
			free(node->args[i++]);
		free(node->args);
	}
	free_redirections(node->redirects);
	if (node->heredoc_data)
		free_heredoc_data(node->heredoc_data);
	free_ast_node(node->left);
	free_ast_node(node->right);
	free(node);
}*/

void free_heredoc_data(t_heredoc_data *heredoc)
{
	int i;

	if (!heredoc)
		return;
	if (heredoc->lines)
	{
		for (i = 0; i < heredoc->count; i++)
			free(heredoc->lines[i]);
		free(heredoc->lines);
	}
	free(heredoc->content);
	free(heredoc->temp_file);
	free(heredoc->delimiter);
	free_tokens(heredoc->token);
	free(heredoc);
}

void free_all(t_minishell_data *data)
{
    if (!data)
        return;
    if (data->env)
        free_env(data->env);
    if (data->tokens)
        free_tokens(data->tokens);
    if (data->ast)
        free_ast_node(data->ast);
    if (data->redirects)
        free_redirects(data->redirects);
    if (data->heredoc)
        free_heredoc_data(data->heredoc);
}

