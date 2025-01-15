#include "minishell.h"

t_ast_node *parse_simple_command(t_token **current)
{
    t_ast_node *node;
    t_redirect *redirects;
    char **args;
    int arg_count;
    t_token *start;

    redirects = NULL;
    start = *current;
    arg_count = count_args(start);
    args = create_args_array(arg_count);
    if (!args)
        return (NULL);
    if (!copy_token_values(args, start, arg_count))
    {
        free_args_array(args);
        return (NULL);
    }
    node = handle_command_redirects(current, args, &redirects);
    return (node);
}

t_ast_node *parse_pipe_sequence(t_token **current)
{
    t_ast_node *left;
    t_ast_node *right;

    left = parse_simple_command(current);
    if (!left)
        return (NULL);
    if (!*current || (*current)->type != TOKEN_PIPE)
        return (left);
    *current = (*current)->next;
    right = parse_pipe_sequence(current);
    if (!right)
    {
        free_ast_node(left);
        return (NULL);
    }
    return (create_pipe_sequence(left, right));
}

t_ast_node *parse_command(const char *input, t_env *env)
{
    t_token *tokens;
    t_token *current;
    t_ast_node *ast;

    tokens = tokenize(input, env);
    if (!tokens)
        return (NULL);
    
    current = tokens;
    ast = parse_pipe_sequence(&current);
    free_tokens(tokens);
    return (ast);
}

t_ast_node  *build_ast(t_token *tokens)
{
    t_token     *current;
    t_ast_node  *ast;

    if (!tokens)
        return (NULL);
    current = tokens;
    ast = parse_pipe_sequence(&current);
    return (ast);
}
