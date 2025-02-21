/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                  		:+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:19:07 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:19:11 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

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

void	setup_signals(t_env *env);
void	reset_signals(void);
void	setup_child_signals(void);
void	setup_parent_signals(t_env *env);
void	ignore_signals(void);
int		handle_pending_signals(void);
void	setup_heredoc_signals(void);
void	reset_sighandlers(t_env *env);
void	handle_sigint(int sig, siginfo_t *siginfo, void *f);
void	handle_signal_child(int sig);

#endif
