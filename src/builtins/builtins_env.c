#include "minishell.h"

static char	*process_single_char(char input_char, char *result)
{
	char	*temp;
	char	*new_result;

	temp = malloc(2);
	if (!temp)
		return (result);
	temp[0] = input_char;
	temp[1] = '\0';
	new_result = ft_strjoin(result, temp);
	free(temp);
	free(result);
	return (new_result);
}

static char	*process_env_variable(const char *input, size_t *i,
				char *result, t_env *env)
{
	size_t	start;
	size_t	var_len;
	char	*var_name;
	char	*var_value;
	char	*temp;

	start = *i + 1;
	var_len = 0;
	while (ft_isalnum(input[start + var_len]) || input[start + var_len] == '_')
		var_len++;
	var_name = ft_strndup(&input[start], var_len);
	if (var_name)
	{
		var_value = get_env_value(var_name, env);
		if (var_value)
		{
			temp = ft_strjoin(result, var_value);
			free(var_value);
			free(result);
			result = temp;
		}
		free(var_name);
	}
	*i = *i + var_len + 1;
	return (result);
}

char	*expand_env_variables(const char *input, t_env *env)
{
	char	*result;
	size_t	i;
	size_t	input_len;

	i = 0;
	if (!input || !env)
		return (NULL);
	input_len = ft_strlen(input);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (i < input_len)
	{
		if (input[i] == '$' && (ft_isalpha(input[i + 1])
				|| input[i + 1] == '_'))
			result = process_env_variable(input, &i, result, env);
		else
		{
			result = process_single_char(input[i], result);
			i++;
		}
	}
	return (result);
}

static int	should_print_var(const char *var)
{
	return (ft_strchr(var, '=') != NULL);
}

int	builtin_env(char **environ)
{
	int	i;

	if (!environ)
		return (1);
	i = 0;
	while (environ[i])
	{
		if (should_print_var(environ[i]))
			ft_putendl_fd(environ[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}
