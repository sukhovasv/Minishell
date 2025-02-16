#include "minishell.h"

static int	handle_cd_error(void)
{
	perror("cd");
	return (1);
}

int	builtin_cd(char **argv)
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
