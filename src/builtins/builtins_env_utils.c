#include "minishell.h"

int	update_existing_variable(t_env *env, char *key, char *value)
{
	int		i;
	int		key_len;
	char	*new_var;

	i = 0;
	key_len = ft_strlen(key);
	while (env->environ[i])
	{
		if (ft_strncmp(env->environ[i], key, key_len) == 0
			&& env->environ[i][key_len] == '=')
		{
			new_var = create_env_string(key, value);
			if (!new_var)
				return (0);
			free(env->environ[i]);
			env->environ[i] = new_var;
			return (1);
		}
		i++;
	}
	return (0);
}

void	add_new_variable(t_env *env, char *key, char *value)
{
	char	*new_var;

	new_var = create_env_string(key, value);
	if (!new_var)
		return ;
	if (!add_env_var(env, new_var))
		free(new_var);
}

char	**allocate_new_environ(char **old_environ, int additional_size)
{
	int		i;
	int		j;
	char	**new_environ;

	i = count_env_vars(old_environ);
	new_environ = ft_calloc(i + additional_size + 1, sizeof(char *));
	if (!new_environ)
		return (NULL);
	j = 0;
	while (j < i)
	{
		new_environ[j] = ft_strdup(old_environ[j]);
		if (!new_environ[j])
		{
			while (--j >= 0)
				free(new_environ[j]);
			free(new_environ);
			return (NULL);
		}
		j++;
	}
	return (new_environ);
}

int	add_env_var(t_env *env, char *new_var)
{
	char	**new_environ;
	int		j;
	int		i;

	new_environ = allocate_new_environ(env->environ, 1);
	if (!new_environ)
		return (0);
	i = count_env_vars(env->environ);
	new_environ[i] = new_var;
	new_environ[i + 1] = NULL;
	j = 0;
	while (env->environ[j])
	{
		free(env->environ[j]);
		j++;
	}
	free(env->environ);
	env->environ = new_environ;
	return (1);
}

void	free_env(t_env *env)
{
	int	i;

	if (!env)
		return ;
	if (env->environ)
	{
		i = 0;
		while (env->environ[i])
		{
			free(env->environ[i]);
			i++;
		}
		free(env->environ);
	}
	free(env);
}
