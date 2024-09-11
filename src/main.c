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

int main(void)
{
    extern char **environ;
    t_env env_struct = {environ};
    char *input;

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
            add_history(input);
            execute_command(input, &env_struct);
        }
        free(input);
    }
    return (0);
}
