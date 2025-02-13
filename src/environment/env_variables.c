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

/*char *get_env_value(const char *var_name, t_env *env)
{
    int i;
    size_t name_len;
    const char *actual_name;
    char *debug_str;

    ft_putstr_fd("\n[DEBUG] get_env_value called with: ", 2);
    debug_str = ft_strdup(var_name);
    if (debug_str)
    {
        ft_putstr_fd(debug_str, 2);
        free(debug_str);
    }
    ft_putstr_fd("\n", 2);

    if (!var_name || !env || !env->environ)
        return NULL;
        
    actual_name = (var_name[0] == '$') ? var_name + 1 : var_name;
    name_len = ft_strlen(actual_name);

    ft_putstr_fd("[DEBUG] Looking for: ", 2);
    debug_str = ft_strdup(actual_name);
    if (debug_str)
    {
        ft_putstr_fd(debug_str, 2);
        free(debug_str);
    }
    ft_putstr_fd("\n", 2);

    i = 0;
    while (env->environ[i])
    {
        ft_putstr_fd("[DEBUG] Checking against: ", 2);
        ft_putstr_fd(env->environ[i], 2);
        ft_putstr_fd("\n", 2);

        if (ft_strncmp(env->environ[i], actual_name, name_len) == 0 
            && env->environ[i][name_len] == '=')
        {
            char *value = ft_strdup(env->environ[i] + name_len + 1);
            ft_putstr_fd("[DEBUG] Found value: ", 2);
            if (value)
                ft_putstr_fd(value, 2);
            ft_putstr_fd("\n", 2);
            return value;
        }
        i++;
    }
    
    ft_putstr_fd("[DEBUG] No value found\n", 2);
    return ft_strdup("");
}*/

/*static char *get_env_var_value(const char *str, size_t start, int len)
{
    char *var_name = ft_strndup(&str[start], len);
    char *value = getenv(var_name);
    free(var_name);

    if (value)
        return ft_strdup(value);
    return ft_strdup("");
}*/

/*char *expand_env_var(const char *str, size_t *i, t_env *env)
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
}*/

static char *get_env_var_value(const char *str, size_t start, int len)
{
    char *var_name = ft_strndup(&str[start], len);
    char *value = getenv(var_name);
    free(var_name);

    if (value)
        return ft_strdup(value);
    return ft_strdup("");
}

char *expand_env_var(const char *str, size_t *i, t_env *env)
{
    char *special_var;
    int len;
    size_t start;

    special_var = expand_special_var(str, i, env);
    if (special_var)
        return special_var;
    (*i)++; // Пропускаем $
    start = *i;
    len = get_env_var_length(str, i);
    if (len == 0)
        return ft_strdup("$");
    return get_env_var_value(str, start, len);
}

/*char *expand_env_variables(const char *input, t_env *env)
{
    char *result;
    char *temp;
    size_t i = 0;
    size_t input_len;
    
    if (!input || !env)
        return NULL;
        
    input_len = ft_strlen(input);
    result = ft_strdup("");
    if (!result)
        return NULL;

    while (i < input_len)
    {
        // Если находим $ и за ним буква или _
        if (input[i] == '$' && (ft_isalpha(input[i + 1]) || input[i + 1] == '_'))
        {
            size_t start = i + 1;  // Пропускаем $
            size_t var_len = 0;
            
            // Получаем длину имени переменной
            while (ft_isalnum(input[start + var_len]) || input[start + var_len] == '_')
                var_len++;
                
            // Получаем имя переменной
            char *var_name = ft_strndup(&input[start], var_len);
            if (var_name)
            {
                // Получаем значение переменной
                char *var_value = get_env_value(var_name, env);
                if (var_value)
                {
                    // Добавляем значение к результату
                    temp = ft_strjoin(result, var_value);
                    free(var_value);
                    free(result);
                    result = temp;
                }
                free(var_name);
            }
            i += var_len + 1;  // +1 для пропуска $
        }
        else
        {
            // Добавляем обычный символ
            temp = malloc(2);
            if (temp)
            {
                temp[0] = input[i];
                temp[1] = '\0';
                char *new_result = ft_strjoin(result, temp);
                free(temp);
                free(result);
                result = new_result;
            }
            i++;
        }
    }
    return result;
}*/

void debug_env(t_env *env)
{
    ft_putstr_fd("\n[DEBUG] Environment variables:\n", 2);
    int i = 0;
    while (env->environ[i])
    {
        char *env_str = ft_strdup(env->environ[i]);
        if (env_str)
        {
            ft_putstr_fd(env_str, 2);
            ft_putstr_fd("\n", 2);
            free(env_str);
        }
        i++;
    }
}

char *expand_env_variables(const char *input, t_env *env)
{
    char *debug_str;
    ft_putstr_fd("\n[DEBUG] Expanding: ", 2);
    debug_str = ft_strdup(input);
    if (debug_str)
    {
        ft_putstr_fd(debug_str, 2);
        free(debug_str);
    }
    ft_putstr_fd("\n", 2);

    debug_env(env);

    char *result;
    char *temp;
    size_t i = 0;
    size_t input_len;
    
    if (!input || !env)
        return NULL;
        
    input_len = ft_strlen(input);
    result = ft_strdup("");
    if (!result)
        return NULL;

    while (i < input_len)
    {
        if (input[i] == '$' && (ft_isalpha(input[i + 1]) || input[i + 1] == '_'))
        {
            char pos_str[20];
            sprintf(pos_str, "%zu", i);
            ft_putstr_fd("[DEBUG] Found variable at position: ", 2);
            ft_putstr_fd(pos_str, 2);
            ft_putstr_fd("\n", 2);

            size_t start = i + 1;
            size_t var_len = 0;
            
            while (ft_isalnum(input[start + var_len]) || input[start + var_len] == '_')
                var_len++;
                
            char *var_name = ft_strndup(&input[start], var_len);
            if (var_name)
            {
                ft_putstr_fd("[DEBUG] Variable name: ", 2);
                ft_putstr_fd(var_name, 2);
                ft_putstr_fd("\n", 2);

                char *var_value = get_env_value(var_name, env);
                if (var_value)
                {
                    ft_putstr_fd("[DEBUG] Variable value: ", 2);
                    ft_putstr_fd(var_value, 2);
                    ft_putstr_fd("\n", 2);

                    temp = ft_strjoin(result, var_value);
                    free(var_value);
                    free(result);
                    result = temp;
                }
                free(var_name);
            }
            i += var_len + 1;
        }
        else
        {
            temp = malloc(2);
            if (temp)
            {
                temp[0] = input[i];
                temp[1] = '\0';
                char *new_result = ft_strjoin(result, temp);
                free(temp);
                free(result);
                result = new_result;
            }
            i++;
        }
    }
    return result;
}
