#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

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

char *get_env_value(const char *var_name, t_env *env);
char *expand_env_var(const char *str, size_t *i, t_env *env);
char *expand_env_variables(const char *input, t_env *env);


#endif
