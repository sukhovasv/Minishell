#include "minishell.h"
#include <termios.h>
#include <readline/readline.h>

volatile sig_atomic_t g_signal_received = 0;

static void handle_sigint(int sig)
{
    (void)sig;
    g_signal_received = SIGINT;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

static void handle_sigint_heredoc(int sig)
{
    (void)sig;
    g_signal_received = SIGINT;
    write(1, "\n", 1);
    exit(130);
}

static void handle_sigquit(int sig)
{
    (void)sig;
}

static void handle_signal_child(int sig)
{
    if (sig == SIGQUIT)
    {
        write(2, "Quit: 3\n", 8);
    }
    exit(128 + sig);
}

void setup_child_signals(void)
{
    signal(SIGINT, handle_signal_child);
    signal(SIGQUIT, handle_signal_child);
}

void setup_parent_signals(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);
}

void setup_heredoc_signals(void)
{
    signal(SIGINT, handle_sigint_heredoc);
    signal(SIGQUIT, SIG_IGN);
}

void setup_signals(void)
{
    struct termios term;

    tcgetattr(0, &term);
    term.c_lflag &= ~ECHOCTL;
    tcsetattr(0, TCSANOW, &term);
    setup_parent_signals();
}

void reset_signals(void)
{
    struct termios term;

    tcgetattr(0, &term);
    term.c_lflag |= ECHOCTL;
    tcsetattr(0, TCSANOW, &term);
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
        rl_on_new_line();
        rl_redisplay();
    }
    g_signal_received = 0;
}
