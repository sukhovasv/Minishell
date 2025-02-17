#include "minishell.h"

int	is_str_digit(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_echo(char **argv)
{
	int	i;
	int	n_flag;

	n_flag = 0;
	i = 1;
	if (argv[1] && ft_strncmp(argv[1], "-n", 2) == 0)
	{
		n_flag = 1;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

/*int	builtin_exit(char **argv)
{
	int	status;

	status = 0;
	if (argv[1])
	{
		if (!is_str_digit(argv[1]))
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(argv[1], 2);
			ft_putendl_fd(": numeric argument required", 2);
			exit(255);
		}
		status = ft_atoi(argv[1]);
		if (argv[2])
		{
			ft_putendl_fd("exit: too many arguments", 2);
			return (1);
		}
	}
	exit(status & 0xFF);
}*/

int builtin_exit(t_minishell_data *data, char **argv, int *exitcode)
{
    int		status = 0;
	char	*nptr;
	int 	negative;

	ft_putstr_fd("exit: ", STDERR_FILENO);
	if (argv[1])
	{
		negative = 1;
		nptr = argv[1];
		if (*nptr == '+' || *nptr == '-')
			if (*nptr++ == '-')
				negative *= -1;
		if (!is_str_digit(nptr) || ft_strlen(nptr) > 19)
		{
			ft_putstr_fd(argv[1], STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			free_all(data);  // Очистка памяти перед выходом
			exit(2);
		}
		status = ft_atoi(nptr) * negative;
		if (argv[2])
		{
			ft_putendl_fd("exit: too many arguments", STDERR_FILENO);
			return (1);
		}
	}
	else if (exitcode)
		status = *exitcode;
	free_all(data);  // Очистка памяти перед выходом
    exit(status & 0xFF);
}

void builtin_exit_wrapper(t_env *env, int status)
{
    t_minishell_data data;

	ft_memset(&data, 0, sizeof(t_minishell_data)); // Инициализируем все поля нулями
    data.env = env;
	builtin_exit(&data, (char *[]) {"exit", NULL}, &status);
}
