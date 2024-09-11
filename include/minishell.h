/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 12:31:37 by yberezhn          #+#    #+#             */
/*   Updated: 2023/11/07 14:42:22 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "libft.h"
#include <readline/readline.h>
#include <readline/history.h>

typedef struct {
    char **env;
} t_env;

void handle_signals(int sig);
void setup_signals(void);
void execute_command(char *input, t_env *env_struct);
int is_builtin(char *cmd);
void execute_builtin(char **argv, t_env *env_struct);
void builtin_cd(char **argv);
void builtin_pwd(void);
void builtin_echo(char **argv);
char *allocate_env_var(const char *key, const char *value);
int update_env_var(t_env *env_struct, int i, const char *key, const char *value);
int update_existing_variable(t_env *env_struct, const char *key, const char *value);
int count_env_vars(char **env);
char **allocate_new_environ(int env_size);
void copy_existing_vars(char **new_environ, char **old_environ, int env_size);
void create_new_var(char **new_environ, int env_size, const char *key, const char *value);
void add_new_variable(t_env *env_struct, const char *key, const char *value);
void builtin_export(char **argv, t_env *env_struct);
void builtin_unset(char **argv, t_env *env_struct);
void builtin_env(char **environ);
void handle_redirections(char **argv);
int handle_output_redirection(void);
int handle_append_redirection(void);
int handle_input_redirection(void);
void initialize_pipe(int p[2]);
void read_and_write_heredoc(void);
void setup_heredoc_redirection(void);
int handle_heredoc_redirection(void);
void handle_pipes(char *input);
void print_argument(char *arg);
void print_arguments(char **argv, int n_flag);
int check_n_flag(char **argv);


#endif //MINISHELL_H
