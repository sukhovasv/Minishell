#include "minishell.h"

char	*extract_var_name(const char *input, int *i)
{
	int	start;
	int	len;

	len = 0;
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
	{
		len++;
		(*i)++;
	}
	return (ft_strndup(&input[start], len));
}

char	*process_dollar(const char *input, int *i, t_env *env)
{
	char	*var_name;
	char	*value;

	(*i)++;
	var_name = extract_var_name(input, i);
	if (!var_name)
		return (NULL);
	value = find_env_value(var_name, ft_strlen(var_name), env);
	free(var_name);
	return (value);
}

static void	handle_quotes_state(t_quote_state *state, const char *input,
			int *in_single, int *in_double)
{
	handle_quote(input[state->i], in_single, in_double);
	state->in_quotes = (*in_single || *in_double);
	if (state->in_quotes)
		state->quote_type = input[state->i];
	else
		state->quote_type = 0;
	state->i++;
}

static void	process_input_string(const char *input, t_quote_state *state,
			t_env *env)
{
	int				in_single;
	int				in_double;
	t_str_processor	proc;

	proc.result = state->result;
	proc.i = (int *)&state->i;
	proc.j = (int *)&state->j;
	in_single = 0;
	in_double = 0;
	while (input[state->i])
	{
		if ((input[state->i] == '\'' || input[state->i] == '"')
			&& (!state->in_quotes || input[state->i] == state->quote_type))
		{
			handle_quotes_state(state, input, &in_single, &in_double);
			continue ;
		}
		if (handle_env_variable(input, &proc, env, state->in_quotes))
			continue ;
		else
			state->result[state->j++] = input[state->i++];
	}
	state->result[state->j] = '\0';
}

char	*parse_quotes(const char *input, t_env *env)
{
	t_quote_state	state;

	state.result = allocate_result_buffer(input);
	if (!state.result)
		return (NULL);
	state.in_quotes = 0;
	state.quote_type = 0;
	state.i = 0;
	state.j = 0;
	state.len = ft_strlen(input);
	process_input_string(input, &state, env);
	return (state.result);
}
