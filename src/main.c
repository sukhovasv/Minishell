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

static void init_shell(t_env **env, t_fd_info *fd_info, int *is_interactive)
{
   extern char **environ;

   *env = init_env(environ);
   setup_signals();
   *is_interactive = isatty(STDIN_FILENO);
   fd_info->saved_stdout = -1;
   fd_info->saved_stdin = -1;
}

static void cleanup_resources(char *input, char *processed_input, 
                          t_ast_node *ast, t_token *tokens)
{
   if (ast)
       free_ast_node(ast);
   if (tokens)
       free_tokens(tokens);
   if (processed_input)
       free(processed_input);
   if (input)
       free(input);
}

static int handle_input(char *input, int is_interactive)
{
   if (!input)
   {
       if (g_signal_received != SIGINT)
       {
           if (is_interactive)
               write(1, "exit\n", 5);
           return (0);
       }
       g_signal_received = 0;
       return (1);
   }
   return (2);
}

static void process_command(char *processed_input, t_env *env, 
                          t_fd_info *fd_info, int is_interactive)
{
    t_token *tokens;
    t_ast_node *ast;
    int status;

    if (is_interactive)
        add_history(processed_input);
    
    tokens = tokenize(processed_input, env);
    if (tokens)
    {
        ast = build_ast(tokens);
        if (ast)
        {
            status = execute_ast_node(ast, env, fd_info);  // Убрал &
            env->last_status = status;
            if (g_signal_received)
                handle_pending_signals();
            free_ast_node(ast);
        }
        free_tokens(tokens);
    }
}

static void cleanup_exit(t_env *env)
{
    reset_signals();
    free_env(env);
}

static void process_valid_input(char *input, char *processed_input,
                              t_env *env, t_fd_info *fd_info, int is_interactive)
{
    process_command(processed_input, env, fd_info, is_interactive);
    cleanup_resources(input, processed_input, NULL, NULL);
}

static void shell_loop(t_env *env, t_fd_info *fd_info, int is_interactive)
{
    char *input;
    char *processed_input;
    int input_status;

    while (1)
    {
        input = readline(is_interactive ? "minishell> " : "");
        input_status = handle_input(input, is_interactive);
        if (input_status == 0)
            break;
        if (input_status == 1)
            continue;
        if (*input && (processed_input = handle_unclosed_quotes(input)))
            process_valid_input(input, processed_input, env, fd_info, is_interactive);
        else
            free(input);
    }
}

int main(void)
{
    t_env *env;
    t_fd_info fd_info;
    int is_interactive;

    init_shell(&env, &fd_info, &is_interactive);
    if (!env)
        return (1);
    shell_loop(env, &fd_info, is_interactive);
    cleanup_exit(env);
    return (0);
}