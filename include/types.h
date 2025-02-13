#ifndef TYPES_H
# define TYPES_H

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

// Глобальные переменные
extern volatile sig_atomic_t g_signal_received;

typedef struct s_command {
    char **argv;
} t_command;

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
	int has_quotes;
	char                *heredoc_content;
    char                *temp_file; 
}   t_token;

typedef struct s_redirect
{
    int                 type;
    char                *file;
    struct s_redirect   *next;
	t_token *token;
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
	int		expand_vars;
	t_token *token;
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

typedef struct s_pipe_data {
    int heredoc_fd;
    int cmd_index;
} t_pipe_data;

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

typedef struct s_termios
{
    struct termios   original;
    struct termios   custom;
} t_termios;

// Типы узлов AST
typedef enum e_ast_type
{
    AST_COMMAND,     // Простая команда (ls, cat, etc)
    AST_PIPE,        // Пайп (|)
    AST_REDIRECT,    // Редирект (<, >, >>, <<)
    //AST_SEQUENCE     // Последовательность команд (;)
} t_ast_type;

// Структура узла AST
typedef struct s_ast_node
{
    t_ast_type           type;           
    struct s_ast_node    *left;          
    struct s_ast_node    *right;         
    char                 **args;         
    t_redirect           *redirects;     
    int                 pipe_level;
    t_heredoc_data      *heredoc_data;   // новое поле
    int                 heredoc_count;    // новое поле
} t_ast_node;

#endif
