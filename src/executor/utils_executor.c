#include "minishell.h"

int	try_direct_execution(char **argv, char *fullpath)
{
	int	errcode;

	errcode = -1;
	if ((*argv[0] == '/') || *argv[0] == '.')
		errcode = access(ft_strncpy(fullpath, argv[0], PATH_MAX), F_OK);
	return (errcode);
}

char	*get_path_env(char **argv)
{
	char	*path_env;
	char	*path;

	path_env = getenv("PATH");
	if (!path_env)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putendl_fd(": No such file or directory", 2);
		exit(127);
	}
	path = ft_strdup(path_env);
	if (!path)
		exit(1);
	return (path);
}
