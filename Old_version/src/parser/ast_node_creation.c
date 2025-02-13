#include "minishell.h"

t_ast_node *create_command_node(char **args, t_redirect *redirects)
{
    t_ast_node *node;

    node = ft_calloc(1, sizeof(t_ast_node));
    if (!node)
        return (NULL);
    node->type = AST_COMMAND;
    node->args = args;
    node->redirects = redirects;
    return (node);
}

/*t_ast_node *create_pipe_node(t_ast_node *left, t_ast_node *right)
{
    t_ast_node *node;

    node = ft_calloc(1, sizeof(t_ast_node));
    if (!node)
        return (NULL);
    node->type = AST_PIPE;
    node->left = left;
    node->right = right;
    return (node);
}*/

/*t_ast_node *create_pipe_node(t_ast_node *left, t_ast_node *right)
{
   t_ast_node *node;
   
   printf("DEBUG: Creating pipe node\n");
   printf("DEBUG: Left node redirects: %p\n", (void*)left->redirects);
   printf("DEBUG: Right node redirects: %p\n", (void*)right->redirects);

   node = malloc(sizeof(t_ast_node));
   if (!node)
       return NULL;

   node->type = AST_PIPE;
   node->left = left;
   node->right = right;
   node->args = NULL;
   node->redirects = NULL;

   printf("DEBUG: Created pipe node: %p\n", (void*)node);
   printf("DEBUG: After creation - Left node redirects: %p\n", (void*)left->redirects);
   return node;
}*/

/*t_ast_node *create_pipe_node(t_ast_node *left, t_ast_node *right)
{
    t_ast_node *node;
    
    printf("DEBUG: Creating pipe node:\n");
    printf("DEBUG: Left command: %s\n", 
           left && left->args ? left->args[0] : "unknown");
    printf("DEBUG: Left redirects: %p\n", 
           left ? (void*)left->redirects : NULL);
    printf("DEBUG: Right command: %s\n", 
           right && right->args ? right->args[0] : "unknown");

    node = malloc(sizeof(t_ast_node));
    if (!node)
        return NULL;

    node->type = AST_PIPE;
    node->left = left;
    node->right = right;
    node->args = NULL;
    node->redirects = left ? left->redirects : NULL;  // Сохраняем редиректы левой команды

    printf("DEBUG: Pipe node created with redirects: %p\n", 
           (void*)node->redirects);
    return node;
}*/

t_ast_node *create_pipe_node(t_ast_node *left, t_ast_node *right)
{
    t_ast_node *node;
    
    /*printf("DEBUG: Creating pipe node between '%s' and '%s'\n", 
           left && left->args ? left->args[0] : "unknown",
           right && right->args ? right->args[0] : "unknown");*/

    node = malloc(sizeof(t_ast_node));
    if (!node)
        return NULL;

    node->type = AST_PIPE;
    node->left = left;
    node->right = right;
    
    // Не копируем аргументы и редиректы, оставляем их в left node
    node->args = NULL;
    node->redirects = NULL;

    return node;
}

t_ast_node *create_redirect_node(t_ast_node *cmd, t_redirect *redir)
{
    t_ast_node *node;

    node = ft_calloc(1, sizeof(t_ast_node));
    if (!node)
        return (NULL);
    node->type = AST_REDIRECT;
    node->left = cmd;
    node->redirects = redir;
    return (node);
}
