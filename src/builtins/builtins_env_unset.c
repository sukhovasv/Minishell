#include "minishell.h"

/*
void remove_variable(t_env *env_struct, const char *key)
Удаляет переменную окружения из списка переменных env_struct->env,
если такая переменная существует. Она ищет переменную по имени (key)
, и если находит её, удаляет, сдвигая все последующие переменные на одну
позицию назад.
key_len = ft_strlen(key)
Определение длины имени переменной, чтобы использовать её для
сравнения с элементами в массиве переменных окружения.
while (env_struct->env[i])
Начало цикла, который перебирает все переменные окружения, пока не встретит конец списка.
if (ft_strncmp(env_struct->env[i], key, key_len) == 0 && env_struct->env[i][key_len] == '=')
Проверка, совпадает ли текущий элемент массива с искомым ключом.
ft_strncmp(env_struct->env[i], key, key_len) == 0: Проверяет, совпадают ли первые key_len
символов строки env_struct->env[i] и ключа key.
env_struct->env[i][key_len] == '='
Проверяет, что следующий символ после ключа в строке переменной окружения — это знак =.
free(env_struct->env[i]);
Если найден нужный ключ, выделенная под него память освобождается.
while (env_struct->env[j])
Начало внутреннего цикла, который сдвигает все элементы массива после удаленного на одну позицию назад.
env_struct->env[j] = env_struct->env[j + 1];
Сдвиг элементов массива. Элемент env_struct->env[j] получает значение следующего элемента
env_struct->env[j + 1].
j++;
Переход к следующему элементу массива.
Завершение функции после удаления переменной и сдвига элементов.
i++;
Если текущая переменная не соответствует ключу, переход к следующей.

void builtin_unset(char **argv, t_env *env_struct)
проходит по списку переданных имен переменных и вызывает функцию удаления для каждой из них.
два аргумента: argv: аргументы, переданные пользователем.
Первый элемент (argv[0]) — это команда unset, последующие — это имена переменных окружения, которые нужно удалить.
env_struct: структура, которая содержит массив переменных окружения (в формате "ключ=значение")
i = 1 -пропускаем саму команду unset
while (argv[i])
Цикл проходит по всем переданным аргументам (переменным окружения, которые нужно удалить).
remove_variable(env_struct, argv[i]);
Внутри цикла вызывается функция remove_variable, которая отвечает за удаление переменной окружения.

Функция builtin_env  отображает всех переменные окружения.
один аргумент - environ (указатель на массив строк, из переменных окружения в формате "ключ=значение")
Просто проходим по всему массиву и печатаем все переменные по порядку.
*/

void remove_variable(t_env *env_struct, const char *key)
{
    int i;
    int j;
    int key_len;

    i = 0;
    key_len = ft_strlen(key);

    while (env_struct->env[i])
    {
        if (ft_strncmp(env_struct->env[i], key, key_len) == 0 && env_struct->env[i][key_len] == '=')
        {
            free(env_struct->env[i]);
            j = i;
            while (env_struct->env[j])
            {
                env_struct->env[j] = env_struct->env[j + 1];
                j++;
            }
            return ;
        }
        i++;
    }
}


void builtin_unset(char **argv, t_env *env_struct)
{
    int i;
    
    i = 1;
    while (argv[i])
    {
        remove_variable(env_struct, argv[i]);
        i++;
    }
}

void builtin_env(char **environ)
{
    int i;
    
    i = 0;
    while (environ[i])
    {
        printf("%s\n", environ[i]);
        i++;
    }
}