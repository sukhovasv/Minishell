#include "minishell.h"
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

/*static void init_shell(t_env **env, t_fd_info *fd_info, int *is_interactive)
{
    extern char **environ;

    *env = init_env(environ);
    setup_signals();
    *is_interactive = isatty(STDIN_FILENO);  // Возвращаемся к простой проверке
    fd_info->saved_stdout = -1;
    fd_info->saved_stdin = -1;
}*/

/*static char *get_input_prompt(int is_interactive)
{
    if (is_interactive)
        return readline("minishell> ");
    return readline("");
}*/

/*static char *get_input_prompt(int is_interactive)
{
    char *input = NULL;

    if (is_interactive)
        input = readline("minishell> ");
    else
        input = readline("");

    return input;
}*/

/*static char *get_input_prompt(int is_interactive)
{
    // В неинтерактивном режиме не показываем промпт
    if (!is_interactive)
        return readline("");
    return readline("minishell> ");
}*/

static char *get_input_prompt(int is_interactive)
{
    char *line = NULL;
    char buf[4096];
    
    if (!is_interactive) {
        // Читаем напрямую из stdin без использования readline
        if (fgets(buf, sizeof(buf), stdin)) {
            size_t len = strlen(buf);
            if (len > 0 && buf[len-1] == '\n')
                buf[len-1] = '\0';
            line = strdup(buf);
        }
        return line;
    }
    
    return readline("minishell> ");
}

/*static char *get_input_prompt(int is_interactive)
{
    char *line;
    static int heredoc_mode = 0;  // Флаг для отслеживания режима heredoc

    if (!is_interactive) {
        line = readline("");  // Вернемся к readline даже в неинтерактивном режиме
        if (line && strstr(line, "<<")) {
            heredoc_mode = 1;
        } else if (heredoc_mode && line && strchr(line, '>')) {
            printf("> ");  // Только в heredoc выводим промпт
        }
        return line;
    }
    
    if (heredoc_mode) {
        line = readline("> ");
        if (line && strstr(line, "EOF")) {
            heredoc_mode = 0;
        }
    } else {
        line = readline("minishell> ");
        if (line && strstr(line, "<<")) {
            heredoc_mode = 1;
        }
    }
    return line;
}*/

/*static void process_command(char *input, t_env *env, t_fd_info *fd_info, int is_interactive)
{
    t_token *tokens;
    t_ast_node *ast;
    int status;

    if (!input || !*input)
        return;

    if (is_interactive)
        add_history(input);
    
    tokens = tokenize(input, env);
    if (tokens)
    {
        ast = build_ast(tokens);
        if (ast)
        {
            status = execute_ast_node(ast, env, fd_info);
            env->last_status = status;
            free_ast_node(ast);
        }
        free_tokens(tokens);
    }
}*/

static void process_command(char *input, t_env *env, t_fd_info *fd_info, int is_interactive)
{
    t_token *tokens;
    t_ast_node *ast;
    int status;

    if (!input || !*input)
        return;

    if (is_interactive)
        add_history(input);
    
    tokens = tokenize(input, env);
    if (tokens)
    {
        ast = build_ast(tokens);
        if (ast)
        {
            status = execute_ast_node(ast, env, fd_info);
            env->last_status = status;
            free_ast_node(ast);
        }
        free_tokens(tokens);
    }
}

static void shell_loop(t_env *env, t_fd_info *fd_info, int is_interactive)
{
    char *input;

    while (1)
    {
        input = get_input_prompt(is_interactive);
        
        if (!input)
        {
            printf("\n");
            break;
        }

        if (g_signal_received == SIGINT)
        {
            g_signal_received = 0;
            free(input);
            continue;
        }

        if (*input)
        {
            process_command(input, env, fd_info, is_interactive);
            rl_set_prompt("");    // Сбрасываем промпт
            rl_already_prompted = 0;  // Сбрасываем флаг промпта
        }
        
        free(input);
    }
}

int main(void)
{
    t_env *env;
    t_fd_info fd_info;
    int is_interactive;
    struct termios original;

    tcgetattr(STDIN_FILENO, &original);
    init_shell(&env, &fd_info, &is_interactive);
    if (!env)
        return 1;

    shell_loop(env, &fd_info, is_interactive);

    tcsetattr(STDIN_FILENO, TCSANOW, &original);
    //rl_clear_history();
	clear_history();
    free_env(env);

    return 0;
}
