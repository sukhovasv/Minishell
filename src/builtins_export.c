#include "../include/minishell.h"

/*
Функция builtin_export
Реализует команду export, которая используется для добавления
или изменения переменных окружения в оболочке.
Например, при вводе export VAR=value, функция, добавляет или изменяет
переменную окружения VAR, устанавливая её значение в value.
char **argv — массив аргументов командной строки (переменные окружения в формате ключ=значение)
*/

void builtin_export(char **argv, t_env *env_struct)
{
    int i;
    char *equal_sign;
    const char *key;
    const char *value;

    i = 1; //пропускаем саму команду
    while (argv[i])
    {
        equal_sign = ft_strchr(argv[i], '='); //ищем символ "="
        if (equal_sign)
        {
            *equal_sign = '\0'; //чтобы разделить строку на ключ и значение. Теперь argv[i] содержит только ключ (так как строка была разделена на две).
            key = argv[i]; //сохраняем указатель на ключ
            value = equal_sign + 1; //сохраняем значение как часть строки сразу после "="
            if (!update_existing_variable(env_struct, key, value)) //если переменная уже существует, то обновляем ее
                add_new_variable(env_struct, key, value); //если переменная не существует, то создаем новую
            *equal_sign = '='; //восстанавливаем символ "=", чтобы строка была корректной
        }
        i++;
    }
}