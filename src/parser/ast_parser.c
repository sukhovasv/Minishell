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
    
    if (!start)
        return NULL;

    arg_count = count_args(start);
    
    args = create_args_array(arg_count);
    if (!args)
        return NULL;
        
    if (!copy_token_values(args, start, arg_count))
    {
        free_args_array(args);
        return NULL;
    }

    node = handle_command_redirects(current, args, &redirects);
    return node;
}

t_ast_node *parse_pipe_sequence(t_token **current)
{
    t_ast_node *left = parse_simple_command(current);
    if (!left || !*current)
        return left;

    if ((*current)->type == TOKEN_PIPE)
    {
        *current = (*current)->next;
        
        t_ast_node *right = parse_pipe_sequence(current);
        if (!right)
        {
            free_ast_node(left);
            return NULL;
        }
        //printf("'%s'\n", right->args ? right->args[0] : "null");

        /*printf("DEBUG: Creating pipe node between '%s' and '%s'\n", 
               left->args ? left->args[0] : "unknown", 
               right->args ? right->args[0] : "unknown");*/

        t_ast_node *pipe_node = create_pipe_node(left, right);
        if (!pipe_node)
        {
            free_ast_node(left);
            free_ast_node(right);
            return NULL;
        }
        return pipe_node;
    }
    return left;
}

t_ast_node *parse_command(const char *input, t_env *env)
{
    t_token *tokens;
    t_token *current;
    t_ast_node *ast;

    tokens = tokenize(input, env);
    if (!tokens)
        return NULL;
    
    current = tokens;
    ast = parse_pipe_sequence(&current);
    free_tokens(tokens);
    return ast;
}

t_ast_node *build_ast(t_token *tokens)
{
    t_token *current;
    t_ast_node *ast;

    if (!tokens)
        return NULL;
        
    current = tokens;
    ast = parse_pipe_sequence(&current);
    return ast;
}
