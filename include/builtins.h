#ifndef BUILTINS_H
# define BUILTINS_H

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

int is_str_digit(const char *str);
int builtin_echo(char **argv);
int builtin_cd(char **argv);
int builtin_pwd(void);
int builtin_exit(char **argv);
void remove_variable(t_env *env_struct, const char *key);
void copy_existing_vars(char **new_environ, char **old_environ, int env_size);
int update_existing_variable(t_env *env, char *key, char *value);
void add_new_variable(t_env *env, char *key, char *value);
char **allocate_new_environ(char **old_environ, int additional_size);
int add_env_var(t_env *env, char *new_var);
t_env *init_env(char **system_environ);
void free_env(t_env *env);
int builtin_env(char **environ);
void print_sorted_env(t_env *env);
int builtin_export(char **argv, t_env *env);
int builtin_unset(char **argv, t_env *env);
char *create_env_string(const char *key, const char *value);
int is_valid_env_name(const char *str);
size_t count_env_vars(char **env);
int is_builtin(const char *cmd);
int execute_builtin(char **argv, t_env *env);

#endif
