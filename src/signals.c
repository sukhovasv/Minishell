#include "minishell.h"
#include <termios.h>
#include <readline/readline.h>

volatile sig_atomic_t g_signal_received = 0;

static void restore_terminal_settings(int fd)
{
    struct termios term;

    tcgetattr(fd, &term);
    term.c_lflag |= ECHOCTL;
    tcsetattr(fd, TCSANOW, &term);
}

static void disable_terminal_control_chars(int fd)
{
    struct termios term;

    tcgetattr(fd, &term);
    term.c_lflag &= ~ECHOCTL;
    tcsetattr(fd, TCSANOW, &term);
}

static void handle_sigint(int sig)
{
    g_signal_received = sig;
    
    write(STDOUT_FILENO, "\n", 1);
    rl_already_prompted = 0;
    rl_forced_update_display();
}

static void handle_sigquit(int sig)
{
    // Ничего не делаем для SIGQUIT
    (void)sig;
}

static void handle_signal_child(int sig)
{
    g_signal_received = sig;
    exit(128 + sig);
}

void setup_child_signals(void)
{;
    signal(SIGINT, handle_signal_child);
    signal(SIGQUIT, handle_signal_child);
}

void setup_parent_signals(void)
{
    struct sigaction sa_int, sa_quit;
    
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;

    if (sigaction(SIGINT, &sa_int, NULL) == -1) {
        perror("ERROR: sigaction SIGINT failed");
        exit(1);
    }
    sa_quit.sa_handler = handle_sigquit;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1) {
        perror("ERROR: sigaction SIGQUIT failed");
        exit(1);
    }
}


void setup_signals(void)
{
    if (isatty(STDIN_FILENO))
        disable_terminal_control_chars(STDIN_FILENO);
    setup_parent_signals();
}

void reset_signals(void)
{
    if (isatty(STDIN_FILENO))
        restore_terminal_settings(STDIN_FILENO);
}

void ignore_signals(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void handle_pending_signals(void)
{
    if (g_signal_received == SIGINT)
    {
        //rl_on_new_line();
        rl_redisplay();
    }
    g_signal_received = 0;
}
