/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:51:24 by ssukhova          #+#    #+#             */
/*   Updated: 2024/07/16 12:52:01 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

//TODO remove after testing
void print_tokens(t_token *list)
{
	t_token *current = list;
	printf("===%s===\n", current->value);
	while (current->next)
	{
		current = current->next;
		printf("===%s===\n", current->value);
	}
}

int main(void)
{
    extern char **environ;
    t_env env_struct = {environ};
    char *input;
	t_token	*tokens;

    setup_signals();
    while (1)
    {
        input = readline("minishell> ");
        if (!input)
        {
            write(1, "exit\n", 5);
            break ;
        }
        if (*input)
        {
			tokens = tokenise(input);
			print_tokens(tokens);
            add_history(input);
            execute_command(input, &env_struct);
        }
        free(input);
    }
    return (0);
}
