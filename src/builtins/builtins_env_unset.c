#include "minishell.h"

/*void	remove_variable(t_env *env_struct, const char *key)
{
	int	i;
	int	j;
	int	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (env_struct->environ[i])
	{
		if (ft_strncmp(env_struct->environ[i], key, key_len) == 0
			&& env_struct->environ[i][key_len] == '=')
		{
			free(env_struct->environ[i]);
			j = i;
			while (env_struct->environ[j])
			{
				env_struct->environ[j] = env_struct->environ[j + 1];
				j++;
			}
			return ;
		}
		i++;
	}
}

void	copy_existing_vars(char **new_environ, char **old_environ, int env_size)
{
	int	i;

	i = 0;
	while (i < env_size)
	{
		new_environ[i] = ft_strdup(old_environ[i]);
		i++;
	}
}*/
