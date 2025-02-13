#include "minishell.h"

static int get_env_var_length(const char *str, size_t *i)
{
    int len = 0;
    while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
    {
        (*i)++;
        len++;
    }
    return len;
}

static char *expand_special_var(const char *str, size_t *i, t_env *env)
{
    if (str[*i] == '?')
    {
        (*i)++;
        return ft_itoa(env->last_status);
    }
    return NULL;
}

/*static char *get_env_var_value(const char *str, size_t start, int len)
{
    char *var_name = ft_strndup(&str[start], len);
    char *value = getenv(var_name);
    free(var_name);

    if (value)
        return ft_strdup(value);
    return ft_strdup("");
}*/

char *get_env_value(const char *var_name, t_env *env)
{
    int i;
    size_t name_len;

    if (!var_name || !env || !env->environ)
        return NULL;
        
    if (var_name[0] == '$')
        var_name++;  // Пропускаем $
        
    name_len = ft_strlen(var_name);
    i = 0;

    while (env->environ[i])
    {
        if (ft_strncmp(env->environ[i], var_name, name_len) == 0 
            && env->environ[i][name_len] == '=')
        {
            return ft_strdup(env->environ[i] + name_len + 1);
        }
        i++;
    }
    return ft_strdup("");
}

char *expand_env_var(const char *str, size_t *i, t_env *env)
{
    char *special_var;
	char *var_name;
    char *value;
    int len;
    size_t start;

    special_var = expand_special_var(str, i, env);
    if (special_var)
        return special_var;
    (*i)++; // Пропускаем $
    start = *i;
    len = get_env_var_length(str, i);
    if (len == 0)
	{
        return ft_strdup("$");
	}
	var_name = ft_strndup(&str[start], len);
    if (!var_name)
        return ft_strdup("");
    value = find_env_value(var_name, len, env);
    free(var_name); // Освобождаем память после использования var_name
    return value;
    //return get_env_var_value(str, start, len);
}

char *expand_env_variables(const char *input, t_env *env)
{
    char *result;
    char *temp;
    size_t i = 0;
    size_t input_len = ft_strlen(input);
    
    result = ft_strdup("");
    if (!result)
        return NULL;

    while (i < input_len)
    {
        if (input[i] == '$' && (ft_isalpha(input[i + 1]) || input[i + 1] == '_'))
        {
            char *expanded_var = expand_env_var(input, &i, env);
            temp = ft_strjoin(result, expanded_var);
            free(expanded_var);
            free(result);
            result = temp;
        }
        else
        {
            temp = ft_strndup(&input[i], 1);
            char *new_result = ft_strjoin(result, temp);
            free(temp);
            free(result);
            result = new_result;
            i++;
        }
    }
    return result;
}
