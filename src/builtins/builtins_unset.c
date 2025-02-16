#include "minishell.h"

static int	find_env_var(char **environ, char *key)
{
	int		i;
	size_t	key_len;

	key_len = ft_strlen(key);
	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(environ[i], key, key_len) == 0
			&& environ[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static void	shift_env_vars(char **environ, int start)
{
	int	i;

	i = start;
	while (environ[i])
	{
		environ[i] = environ[i + 1];
		i++;
	}
}

static int	unset_var(t_env *env, char *key)
{
	int	index;

	if (!is_valid_env_name(key))
	{
		ft_putstr_fd("unset: '", STDERR_FILENO);
		ft_putstr_fd(key, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (1);
	}
	index = find_env_var(env->environ, key);
	if (index >= 0)
	{
		free(env->environ[index]);
		shift_env_vars(env->environ, index);
	}
	return (0);
}

int	builtin_unset(char **argv, t_env *env)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	while (argv[i])
	{
		if (unset_var(env, argv[i]) != 0)
			status = 1;
		i++;
	}
	return (status);
}
