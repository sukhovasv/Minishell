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

void setup_signals(void);
void reset_signals(void);
void setup_child_signals(void);
void setup_parent_signals(void);
void ignore_signals(void);
void handle_pending_signals(void);
void setup_heredoc_signals(void);


#endif
