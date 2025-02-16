#include "minishell.h"

static char	*create_env_string_from_parts(const char *key,
						const char *clean_value, size_t size)
{
	char	*env_str;

	env_str = ft_calloc(size, sizeof(char));
	if (!env_str)
		return (NULL);
	ft_strlcpy(env_str, key, size);
	ft_strlcat(env_str, "=", size);
	ft_strlcat(env_str, clean_value, size);
	return (env_str);
}

static char	*free_and_return_null(char *ptr)
{
	free(ptr);
	return (NULL);
}

char	*create_env_string(const char *key, const char *value)
{
	char	*value_copy;
	char	*clean_value;
	char	*env_str;
	size_t	size;

	if (!key || !value)
		return (NULL);
	value_copy = ft_strdup(value);
	if (!value_copy)
		return (NULL);
	clean_value = process_value(value_copy);
	if (!clean_value)
		return (free_and_return_null(value_copy));
	size = ft_strlen(key) + ft_strlen(clean_value) + 2;
	env_str = create_env_string_from_parts(key, clean_value, size);
	if (!env_str)
		return (free_and_return_null(value_copy));
	free(value_copy);
	return (env_str);
}

int	is_valid_env_name(const char *str)
{
	int	i;

	if (!str || !ft_isalpha(str[0]))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

size_t	count_env_vars(char **env)
{
	size_t	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}
