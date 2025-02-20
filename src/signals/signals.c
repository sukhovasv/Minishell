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
#include <readline/readline.h>

volatile sig_atomic_t	g_signal_received = 0;

static void	setup_sigint_handler(struct sigaction *sa)
{
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = SA_SIGINFO | SA_RESTART;
	sa->sa_sigaction = &handle_sigint;
}

void	setup_parent_signals(t_env *env)
{
	struct sigaction	*sa;
	struct sigaction	*old_sa;
	int					i;
	const	char		(*signals)[5] = &(char [5]){
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
		if (i == 0)
			setup_sigint_handler(sa);
		if (sigaction((int)(*signals)[i], sa, old_sa) != 0)
			builtin_exit_wrapper(env, EXIT_FAILURE);
	}
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
			builtin_exit_wrapper(env, EXIT_FAILURE);
		}
	}
	signal(SIGTSTP, SIG_IGN);
}

void	ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
