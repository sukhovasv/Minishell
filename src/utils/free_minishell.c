/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:29:14 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:29:16 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_heredoc_data(t_heredoc_data *heredoc)
{
	int	i;

	if (!heredoc)
		return ;
	if (heredoc->lines)
	{
		i = 0;
		while (i < heredoc->count)
		{
			free(heredoc->lines[i]);
			i++;
		}
		free(heredoc->lines);
	}
	free(heredoc->content);
	free(heredoc->temp_file);
	free(heredoc->delimiter);
	free_tokens(heredoc->token);
	free(heredoc);
}

void	free_all(t_minishell_data *data)
{
	if (!data)
		return ;
	if (data->env)
	{
		if (data->env->tokens)
			free_tokens(data->env->tokens);
		if (data->env->ast)
			free_ast_node(data->env->ast);
		free_env(data->env);
	}
	if (data->tokens)
		free_tokens(data->tokens);
	if (data->redirects)
		free_redirects(data->redirects);
	if (data->heredoc)
		free_heredoc_data(data->heredoc);
}
