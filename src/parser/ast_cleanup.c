/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_cleanup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:27:53 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:27:55 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_ast_node_nonrec(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->args)
		free_args(node->args);
	if (node->redirects)
		free_redirects(node->redirects);
	free(node);
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
    free_redirects(node->redirects);
    if (node->heredoc_data)  
        free_heredoc_data(node->heredoc_data);
    free_ast_node(node->left);
    free_ast_node(node->right);
    free(node);
}

void	free_redirects(t_redirect *redirects)
{
	t_redirect	*current;
	t_redirect	*next;

	current = redirects;
	while (current)
	{
		next = current->next;
		if (current->file)
			free(current->file);
		free(current);
		current = next;
	}
}

void	free_args_array(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}
