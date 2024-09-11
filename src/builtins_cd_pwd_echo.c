/*
Функция builtin_cd:
Реализует команду cd (change directory), которая изменяет текущий рабочий
каталог на указанный. (Если аргумент не указан, функция переходит в домашний
каталог пользователя).
Параметр функции - массив строк, строки - аргументы командной строки.
1) проверяем, был ли передан аргумент для команды cd (argv[0] - сама команда cd,
а argv[1] - это аргумент команды (путь к директории)). Если argv[1] равно NULL,
значит, аргумент не был передан, и нужно перейти в домашний каталог пользователя.
Для перехода в домашний каталог используем функцию getenv("HOME"). Она возвращает
значение переменной окружения HOME, которая указывает на домашний каталог
пользователя. Если переменная HOME не установлена, getenv вернет NULL.
Если home не равно NULL, вызываем функцию chdir(home), которая изменяет текущий
рабочий каталог на указанный в home.
else if (chdir(argv[1]) != 0):
Выполняется, если был передан аргумент argv[1], то есть указан путь для перехода.
Функция chdir изменяет текущий рабочий каталог на указанный путь.
Если chdir возвращает не 0, значит произошла
ошибка при смене директории (каталог не существует или нет прав на доступ).
Вызываем perror чтобы вывести сообющение об ошибке.

Функция builtin_pwd:
Реализует команду pwd (print working directory), которая выводит текущий
рабочий каталог.
Объявляем буфер (char swd[1024]), чтобы хранить путь к текущему рабочему каталогу.
getcwd (get current working directory) заполняет указанный буфер cwd строкой,
представляющей текущий рабочий каталог.

Функция builtin_echo
Реализует команду echo. Отвечает за вывод строки в консоль,
с возможностью подавления новой строки при использовании флага -n.
print_argument(char *arg)
if (arg[0] == '$' && arg[1] != '\0'): Проверяется, начинается ли аргумент
с символа $. Если да, то программа предполагает, что это переменная окружения.
char *env_value = getenv(&arg[1]) Вызов функции getenv с аргументом,
начиная со второго символа строки arg, чтобы получить значение переменной окружения.
if (env_value) printf("%s", env_value) Если переменная окружения существует,
ее значение выводится на экран. else printf("%s", arg);: Если аргумент не является
переменной окружения, он выводится как есть.
*/

#include "../include/minishell.h"

void builtin_cd(char **argv)
{
    const char *home;

    if (argv[1] == NULL)
    {
        home = getenv("HOME");
        if (home == NULL)
            printf("cd: HOME environment variable not set\n");
        else if (chdir(home) != 0)
            perror("cd");
    }
    else if (chdir(argv[1]) != 0)
        perror("cd");
}

void builtin_pwd(void)
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else
        perror("getcwd");
}

void builtin_echo(char **argv)
{
    int n_flag;
    
    n_flag = check_n_flag(argv);
    print_arguments(argv, n_flag);
    if (!n_flag)
        printf("\n");
}

int check_n_flag(char **argv)
{
    if (argv[1] && ft_strncmp(argv[1], "-n", 2) == 0)
        return (1);
    return (0);
}

void print_arguments(char **argv, int n_flag)
{
    int i;

    if (n_flag)
        i = 2;  // Если флаг -n установлен, начинаем с третьего аргумента
    else
        i = 1;  // Если флаг -n не установлен, начинаем со второго аргумента
    while (argv[i])
    {
        print_argument(argv[i]);
        if (argv[i + 1])
            printf(" ");
        i++;
    }
}

void print_argument(char *arg)
{
    char    *env_value;

    if (arg[0] == '$' && arg[1] != '\0') // Проверяем, начинается ли аргумент с '$'
    {
        env_value = getenv(&arg[1]); // Получаем значение переменной окружения
        if (env_value)
            printf("%s", env_value);
    }
    else
        printf("%s", arg);
}