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

// Debug макрос
/*#ifdef DEBUG
# define DEBUG_PRINT(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
# define DEBUG_PRINT(fmt, ...) (void)0
#endif*/

// Глобальные переменные
extern volatile sig_atomic_t g_signal_received;

/*// Константы для перенаправлений
# define REDIR_IN        1
# define REDIR_OUT       2
# define REDIR_APPEND    3
# define REDIR_HEREDOC   4*/

// Базовые структуры

typedef struct s_parser_state
{
    int in_quotes;
    char quote_char;
} t_parser_state;

typedef struct s_env
{
    char    **environ;
    int     last_status;
}   t_env;

typedef struct s_redirect
{
    int                 type;
    char                *file;
    struct s_redirect   *next;
}   t_redirect;

typedef struct s_heredoc_data
{
    char    **lines;  // массив строк для хранения содержимого
    int     count;    // количество строк
    int     capacity; //размер массива
    char    *content;
    char    *temp_file;
    int     temp_fd;
    char    *delimiter;
} t_heredoc_data;

typedef struct s_heredoc
{
    char    *content;
    char    *temp_file;
    int     temp_fd;
}   t_heredoc;

typedef struct s_fd_info
{
    int saved_stdout;
    int saved_stdin;
} t_fd_info;

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_REDIR_HEREDOC
}   t_token_type;

typedef struct s_token
{
    t_token_type        type;
    char                *value;
    struct s_token      *next;
}   t_token;

typedef struct s_quote_state {
    int     in_quotes;
    char    quote_type;
    char    *result;
    size_t  capacity;
    size_t  length;
    size_t  i;        // текущая позиция в строке
    size_t  j;        // текущая позиция в результате
    size_t  len;      // длина входной строки
} t_quote_state;

// Включаем ast.h после базовых определений
# include "ast.h"

// Функции обработки сигналов
void setup_signals(void);
void reset_signals(void);
void setup_child_signals(void);
void setup_parent_signals(void);
void ignore_signals(void);
void restore_signals(void);
void handle_pending_signals(void);

// Функции парсинга и токенизации
t_token *tokenize(const char *input, t_env *env);
void free_tokens(t_token *tokens);
char *parse_quotes(const char *input, t_env *env);
t_ast_node *parse_command(const char *input, t_env *env);
//void free_command(t_command *cmd);
const char *token_type_to_string(t_token_type type);
int is_word_char(char c);
char *handle_unclosed_quotes(const char *input);
//int is_delimiter(char c);


// Функции выполнения команд
int execute_command(char *input, t_env *env);
int is_builtin(const char *cmd);
int execute_builtin(char **argv, t_env *env);
int handle_redirections(t_redirect *redirects, t_fd_info *fd_info); // В minishell.h
int execute_non_builtin(char **argv, t_env *env);
int  try_execute(char *cmd, char **argv, char **envp);
char *get_path_env(char **argv);
void try_direct_execution(char **argv, char **envp);
void	try_path_execution(char *path, char **argv, char **envp);
void search_and_execute(char **argv, char **envp);
int	handle_redir_append(t_redirect *redir, int *saved_stdout);
int	handle_redir_output(t_redirect *redir, int *saved_stdout);
int	handle_redir_input(t_redirect *redir, int *saved_stdin);
int handle_redir_heredoc(t_redirect *redir, t_fd_info *fd_info);
char *get_env_value(const char *str, t_env *env);

// Встроенные команды
int builtin_cd(char **argv);
int builtin_pwd(void);
int builtin_echo(char **argv);
int builtin_export(char **argv, t_env *env);
int builtin_unset(char **argv, t_env *env);
int builtin_env(char **environ);
int builtin_exit(char **argv);
int check_n_flag(char **argv);
size_t get_env_var_size(const char *key, const char *value);
char *create_env_string(const char *key, const char *value);
int is_valid_env_name(const char *str);
size_t count_env_size(char **env);

// Функции для работы с переменными окружения
void add_new_variable(t_env *env, char *key, char *value);
int update_existing_variable(t_env *env, char *key, char *value);
void remove_variable(t_env *env, const char *key);
char *expand_env_var(const char *str, size_t *i, t_env *env);
int add_env_var(t_env *env, char *new_var);
t_env *init_env(char **system_environ);
void free_env(t_env *env);
t_env *init_env(char **system_environ);
void free_env(t_env *env);

// Вспомогательные функции
char *ft_strndup(const char *s, size_t n);
void ft_putstr_fd(char *s, int fd);
void ft_putendl_fd(char *s, int fd);
size_t ft_strlcpy(char *dst, const char *src, size_t size);
size_t ft_strlcat(char *dst, const char *src, size_t size);
//int is_delimiter(char c);


/* Redirection handlers */
int         handle_redir_input(t_redirect *redir, int *saved_stdin);
int         handle_redir_output(t_redirect *redir, int *saved_stdout);
int         handle_redir_append(t_redirect *redir, int *saved_stdout);
//int handle_redirections(t_redirect *redirects, t_fd_info *fd_info);
void restore_redirections(t_fd_info *fd_info);

int handle_signals_and_fork(t_heredoc_data *heredocs, int count);
int process_heredocs_in_child(t_heredoc_data *heredocs, int count);
int fork_and_process_heredocs(t_heredoc_data *heredocs, int count);
void cleanup_heredoc_files(t_heredoc_data *heredocs, int count);
int free_initialized_heredocs(t_heredoc_data *heredocs, int count);
int open_temp_file(const char *temp_file);
int is_end_of_heredoc(const char *line, const char *delimiter);
int read_and_write_lines(int fd, t_heredoc_data *data);
void setup_input_redirection(int last_fd, t_fd_info *fd_info);
int process_current_heredoc(t_heredoc_data *heredocs, t_redirect *current, int i);
int process_and_open_last_heredoc(t_heredoc_data *heredocs, int count);
int fork_and_process_heredocs(t_heredoc_data *heredocs, int count);
int process_heredocs_in_child(t_heredoc_data *heredocs, int count);
int allocate_heredoc_data(int count, t_heredoc_data **heredocs);
int initialize_single_heredoc(t_heredoc_data *heredoc, t_redirect *current);
int initialize_heredocs(t_redirect *redir, t_heredoc_data *heredocs);
int prepare_heredoc_data(t_redirect *redir, t_heredoc_data **heredocs);
char *create_temp_file(void);
int write_single_heredoc(t_heredoc_data *data);

/* Utils */
void        restore_fds(int saved_stdout, int saved_stdin);
int         handle_command_error(t_ast_node *cmd, int saved_stdout, 
                int saved_stdin);

// Отладочные функции
#ifdef DEBUG
void print_tokens(t_token *list);
#endif

#endif // MINISHELL_H
