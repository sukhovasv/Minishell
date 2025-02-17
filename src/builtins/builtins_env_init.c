#include "minishell.h"

static int	copy_environ_strings(char **dest, char **src, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		dest[i] = ft_strdup(src[i]);
		if (!dest[i])
		{
			while (i > 0)
			{
				i--;
				free(dest[i]);
			}
			return (0);
		}
		i++;
	}
	return (1);
}

t_env	*init_env(char **environ)
{
	t_env	*env;
	int		size;

	size = 0;
	while (environ[size])
		size++;
	env = ft_calloc(1, sizeof(t_env));
	if (!env)
		return (NULL);
	env->environ = malloc(sizeof(char *) * (size + 1));
	if (!env->environ)
	{
		free(env);
		return (NULL);
	}
	if (!copy_environ_strings(env->environ, environ, size))
	{
		free(env->environ);
		free(env);
		return (NULL);
	}
	env->environ[size] = NULL;
	env->last_status = 0;
	return (env);
}
