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

static int	handle_numeric_argument(char *arg, int *status)
{
	char	*nptr;
	int		negative;

	negative = 1;
	nptr = arg;
	if (*nptr == '+' || *nptr == '-')
		if (*nptr++ == '-')
			negative *= -1;
	if (!is_str_digit(nptr) || ft_strlen(nptr) > 19)
		return (0);
	*status = ft_atoi(nptr) * negative;
	return (1);
}

int	builtin_exit(t_minishell_data *data, char **argv, int *exitcode)
{
	int	status;

	status = 0;
	if (argv[1])
	{
		if (!handle_numeric_argument(argv[1], &status))
		{
			ft_putstr_fd(argv[1], STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			free_all(data);
			exit(2);
		}
		if (argv[2])
		{
			ft_putendl_fd("exit: too many arguments", STDERR_FILENO);
			return (1);
		}
	}
	else if (exitcode)
		status = *exitcode;
	free_all(data);
	exit(status & 0xFF);
}

void	builtin_exit_wrapper(t_env *env, int status)
{
	t_minishell_data	data;

	ft_memset(&data, 0, sizeof(t_minishell_data));
	data.env = env;
	builtin_exit(&data, (char *[]){"exit", NULL}, &status);
}
