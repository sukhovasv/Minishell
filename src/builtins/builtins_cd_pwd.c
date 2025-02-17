#include "minishell.h"

static int	handle_cd_error(void)
{
	perror("cd");
	return (1);
}

/*int	builtin_cd(char **argv)
{
	extern char	**environ;
	t_env		*env;
	char		path[PATH_MAX];
	char		*home;

	env = init_env(environ);
	if (!argv[1])
	{
		home = get_env_value("HOME", env);
		if (!home)
		{
			ft_putendl_fd("cd: HOME environment variable not set", 2);
			return (1);
		}
		if (chdir(home) != 0)
			return (handle_cd_error());
	}
	else if (chdir(argv[1]) != 0)
		return (handle_cd_error());
	if (getcwd(path, PATH_MAX))
		update_existing_variable(env, "PWD", path);
	return (0);
}*/

static int cd_handle_home(t_env *env, char *old_pwd)
{
    char *home;

    home = get_env_value("HOME", env);
    if (!home)
    {
        free(old_pwd);
        free_env(env);
        ft_putendl_fd("cd: HOME environment variable not set", 2);
        return (1);
    }
    if (chdir(home) != 0)
    {
        free(home);
        free(old_pwd);
        free_env(env);
        return (handle_cd_error());
    }
    free(home);
    return (0);
}

static int cd_handle_path(t_env *env, char *old_pwd, char *path)
{
    if (chdir(path) != 0)
    {
        free(old_pwd);
        free_env(env);
        return (handle_cd_error());
    }
    return (0);
}

static void cd_update_pwd(t_env *env, char *old_pwd)
{
    char path[PATH_MAX];

    if (old_pwd)
        update_existing_variable(env, "OLDPWD", old_pwd);
    if (getcwd(path, PATH_MAX))
        update_existing_variable(env, "PWD", path);
}

/*int builtin_cd(char **argv)
{
    extern char **environ;
    t_env *env;
    char *old_pwd;
    int status;

    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
        return (1);

    env = init_env(environ);
    if (!env)
    {
        free(old_pwd);
        return (1);
    }

    if (!argv[1])
        status = cd_handle_home(env, old_pwd);
    else
        status = cd_handle_path(env, old_pwd, argv[1]);

    if (status == 0)
        cd_update_pwd(env, old_pwd);

    free(old_pwd);
    free_env(env);
    return (status);
}*/

int builtin_cd(char **argv, t_env *env)  // добавляем env как параметр
{
    char *old_pwd;
    int status;

    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
        return (1);

    if (!argv[1])
        status = cd_handle_home(env, old_pwd);
    else
        status = cd_handle_path(env, old_pwd, argv[1]);

    if (status == 0)
        cd_update_pwd(env, old_pwd);

    free(old_pwd);
    return (status);
}

int	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		return (0);
	}
	perror("pwd");
	return (1);
}
