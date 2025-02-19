#include "minishell.h"

static char	*create_env_string_from_parts(const char *key,
						const char *clean_value)
{
	char	*env_str;
	size_t	size;

	size = ft_strlen(key) + 1;
	if (clean_value)
		size = ft_strlen(key) + ft_strlen(clean_value) + 2;
	env_str = ft_calloc(size, sizeof(char));
	if (!env_str)
		return (NULL);
	ft_strlcpy(env_str, key, size);
	if (clean_value)
	{
		ft_strlcat(env_str, "=", size);
		ft_strlcat(env_str, clean_value, size);
	}
	return (env_str);
}

char	*create_env_string(const char *key, const char *value)
{
	char	*value_copy;
	char	*clean_value;
	char	*env_str;

	if (!key)
		return (NULL);
	clean_value = NULL;
	if (value)
	{
		value_copy = ft_strdup(value);
		if (!value_copy)
			return (NULL);
		clean_value = process_value(value_copy);
		free(value_copy);
		if (!clean_value)
			return (NULL);
	}
	env_str = create_env_string_from_parts(key, clean_value);
	free(clean_value);
	if (!env_str)
		return (NULL);
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
