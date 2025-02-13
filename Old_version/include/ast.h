#ifndef AST_H
# define AST_H

# include "minishell.h"

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

// ast_redirects.c
//t_redirect  *create_redirect(t_token_type type, const char *file);
t_redirect *create_redirect(t_token_type type, const char *file, t_token *token);
int         add_redirect_to_list(t_redirect **redirects, t_redirect *new_redir);
int         is_redirect_token(t_token_type type);
void        free_redirects(t_redirect *redirects);  // Добавили эту строку

// ast_args.c
int         count_args(t_token *tokens);
char        **create_args_array(int count);
int         copy_token_values(char **args, t_token *tokens, int count);
void        free_args_array(char **args);  // Добавили эту строку

// ast_utils.c
t_ast_node  *handle_command_redirects(t_token **current, char **args,
                t_redirect **redirects);
t_ast_node  *create_pipe_sequence(t_ast_node *left, t_ast_node *right);
void        cleanup_command(char **args, t_redirect *redirects);
int         process_redirect(t_token **current, t_token_type type,
                t_redirect **redirects);

// ast_parser.c
t_ast_node *parse_command(const char *input, t_env *env);
t_ast_node  *parse_pipe_sequence(t_token **current);
t_ast_node  *parse_simple_command(t_token **current);
t_ast_node  *build_ast(t_token *tokens);  // Добавили

// ast_node.c
t_ast_node  *create_command_node(char **args, t_redirect *redirects);
t_ast_node  *create_pipe_node(t_ast_node *left, t_ast_node *right);
t_ast_node  *create_redirect_node(t_ast_node *cmd, t_redirect *redir);
void        free_ast_node(t_ast_node *node);

// ast_executor.c
int execute_ast_node(t_ast_node *node, t_env *env, t_fd_info *fd_info);
#endif
