/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:53:05 by ssukhova          #+#    #+#             */
/*   Updated: 2024/07/16 12:53:10 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Функция clean_arguments удаляет пустые элементы (NULL)
из массива аргументов.

Функция execute_non_builtin выполняет не встроенные команды
в child процессе.
fork создает новый процесс, который является копией текущего процесса
(родительского).
pid == -1 Проверка ошибки при создании нового процесса.
pid == 0 Код, выполняемый в дочернем процессе.
Использует execvp для замены текущего процесса новой программой,
указанной в clean_argv[0].
waitpid Родительский процесс ожидает завершения дочернего процесса.

Функция execute_command обработывает и выполненяет введенные команды.
parse_input разделяет строку ввода на массив аргументов. (Пока
просто ft_split, чтобы тестить)
handle_redirections Обрабатывает перенаправления ввода/вывода (не готово)
clean_arguments Очищает массив аргументов от пустых элементов.
is_builtin Проверяет, является ли команда встроенной.
execute_builtin Выполняет встроенную команду.
execute_non_builtin Выполняет внешнюю команду.
*/

#include "minishell.h"

void handle_redirections(char **argv)
{
    int i;

    i = 0;
    while (argv[i])
    {
        if (handle_output_redirection())
            continue ;
        if (handle_append_redirection())
            continue ;
        if (handle_input_redirection())
            continue ;
        if (handle_heredoc_redirection())
            continue ;
        i++;
    }
}

int handle_output_redirection(void)
{
    return 0;
}


int handle_append_redirection(void)
{
    return (0);
}

int handle_input_redirection(void)
{
    return (0);
}

char **parse_input(char *input)
{
    return ft_split(input, ' ');
}

void clean_arguments(char **argv, char **clean_argv)
{
    int i;
    int j;
    
    j = 0;
    i = 0;
    while (argv[i])
    {
        if (argv[i] != NULL)
            clean_argv[j++] = argv[i];
        i++;
    }
    clean_argv[j] = NULL;
}

void execute_non_builtin(char **clean_argv)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return;
    }
    if (pid == 0)
    {
        if (execvp(clean_argv[0], clean_argv) == -1)
        {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    else
        waitpid(pid, NULL, 0);
}

void execute_command(char *input, t_env *env_struct)
{
    char **argv;
    char *clean_argv[1024];

    argv = parse_input(input);
    if (argv == NULL || argv[0] == NULL)
    {
        free(argv);
        return;
    }
    handle_redirections(argv);
    clean_arguments(argv, clean_argv);
    if (is_builtin(clean_argv[0]))
        execute_builtin(clean_argv, env_struct);
    else
        execute_non_builtin(clean_argv);
    free(argv);
}

