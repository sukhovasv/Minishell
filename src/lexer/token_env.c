/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:25:44 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:25:46 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_special_env(const char *str, t_env *env)
{
	char	*status;

	if (!str || !env)
		return (NULL);
	if (str[0] == '?')
	{
		status = ft_itoa(env->last_status);
		if (!status)
			return (ft_strdup("0"));
		while (str[1])
			str++;
		return (status);
	}
	return (NULL);
}

char	*find_env_value(const char *var_name, size_t len, t_env *env)
{
	int		i;
	size_t	env_name_len;

	i = 0;
	while (env->environ[i])
	{
		env_name_len = 0;
		while (env->environ[i][env_name_len]
				&& env->environ[i][env_name_len] != '=')
			env_name_len++;
		if (env_name_len == len
			&& ft_strncmp(env->environ[i], var_name, len) == 0)
		{
			return (ft_strdup(env->environ[i] + len + 1));
		}
		i++;
	}
	return (ft_strdup(""));
}

char	*get_env_value(const char *str, t_env *env)
{
	char	*value;
	char	*var_name;
	size_t	len;

	if (!str || !env || !env->environ)
		return (NULL);
	if (str[0] == '$')
		str++;
	value = handle_special_env(str, env);
	if (value)
		return (value);
	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	if (len == 0)
		return (ft_strdup("$"));
	var_name = ft_strndup(str, len);
	if (!var_name)
		return (NULL);
	value = find_env_value(var_name, len, env);
	free(var_name);
	if (value)
		return (value);
	return (ft_strdup(""));
}

char	*handle_env_var(t_word_parser *parser)
{
	char	*env_val;
	size_t	env_len;

	env_val = get_env_value(parser->str + parser->i, parser->env);
	if (!env_val)
		return (parser->result);
	env_len = ft_strlen(env_val);
	if (parser->j + env_len >= parser->len * 2)
	{
		parser->result = reallocate_result(parser->result, env_val,
				parser->j, env_len);
		if (!parser->result)
			return (NULL);
	}
	copy_env_value(parser->result, env_val, &parser->j);
	free(env_val);
	update_index_after_env_var(parser);
	return (parser->result);
}
