#include "minishell.h"

int is_empty_command(char *line)
{
    if (!line || !*line)
        return (1);
        
    // Проверка на строку из пробелов/табуляций
    while (*line)
    {
        if (*line != ' ' && *line != '\t')
            return (0);
        line++;
    }
    return (1);
}