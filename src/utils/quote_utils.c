/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:29:35 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:29:37 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_quote_state(int *i, int *j, int *in_single_quotes,
			int *in_double_quotes)
{
	*i = 0;
	*j = 0;
	*in_single_quotes = 0;
	*in_double_quotes = 0;
}

char	*allocate_result_buffer(const char *input)
{
	char	*result;

	result = malloc(ft_strlen(input) * 2 + 1);
	if (!result)
		return (NULL);
	return (result);
}

void	handle_quote(char c, int *in_single_quotes, int *in_double_quotes)
{
	if (c == '\'' && !*in_double_quotes)
		*in_single_quotes = !(*in_single_quotes);
	else if (c == '"' && !*in_single_quotes)
		*in_double_quotes = !(*in_double_quotes);
}

char	*handle_env_vars(const char *input, int *i, t_env *env)
{
	if (input[*i] == '$')
		return (process_dollar(input, i, env));
	return (ft_strdup(""));
}

int	handle_env_variable(const char *input, t_str_processor *proc,
			t_env *env, int in_single_quotes)
{
	char	*env_val;

	if (input[*proc->i] == '$' && !in_single_quotes)
	{
		if (input[*proc->i + 1] == '?')
		{
			env_val = ft_itoa(env->last_status);
			(*proc->i) += 2;
		}
		else
			env_val = handle_env_vars(input, proc->i, env);
		if (env_val)
		{
			ft_strlcpy(&proc->result[*proc->j], env_val,
				ft_strlen(env_val) + 1);
			*proc->j += ft_strlen(env_val);
			free(env_val);
		}
		return (1);
	}
	return (0);
}
