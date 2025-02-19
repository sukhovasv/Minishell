/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:28:41 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:28:43 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>
#include <readline/readline.h>

volatile sig_atomic_t g_signal_received = 0;

static void handle_sigint(int sig, siginfo_t *siginfo, void *f)
{
    (void)siginfo;
	(void)f;
    g_signal_received = sig;
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

void setup_parent_signals(t_env *env)
{
	t_sigaction	*sa;
	t_sigaction	*old_sa;
	int 		i;
	const	char	(*signals)[5] = &(char [5]){
			SIGINT,
			SIGTERM,
			SIGQUIT,
			SIGWINCH,
			SIGTSTP
	};

	i = -1;
	while (++i < 5)
	{
		sa = &env->new_sigactions[i];
		old_sa = &env->old_sigactions[i];

		env->new_sigactions[i].sa_handler = SIG_IGN;
		if(i == 0)
		{
			sigemptyset(&sa->sa_mask);
			sa->sa_flags = SA_SIGINFO | SA_RESTART;
			sa->sa_sigaction = &handle_sigint;
		}
		if (sigaction((int)(*signals)[i], sa, old_sa) != 0)
			exit(EXIT_FAILURE); //todo
	}
//    signal(SIGINT, handle_sigint);
//    signal(SIGQUIT, handle_sigquit);
}

void	reset_sighandlers(t_env *env)
{
	int				i;
	const	char	(*signals)[5] = &(char [5]){
			SIGINT,
			SIGTERM,
			SIGQUIT,
			SIGWINCH,
			SIGTSTP
	};

	i = -1;
	while (++i < 5)
	{
		if (sigaction((int)(*signals)[i], &env->old_sigactions[i], NULL))
		{
			exit(EXIT_FAILURE); //todo
		}
	}
}

void setup_heredoc_signals(void)
{
    signal(SIGINT, handle_sigint_heredoc);
    signal(SIGQUIT, SIG_IGN);
}

void setup_signals(t_env *env)
{
    struct termios term;

    tcgetattr(0, &term);
    term.c_lflag &= ~ECHOCTL;
    tcsetattr(0, TCSANOW, &term);
	setup_parent_signals(env);
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
