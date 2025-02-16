#include "minishell.h"

char	*create_temp_file(void)
{
	static unsigned int	temp_count = 0;
	char				*temp_name;
	char				*num_str;

	if (temp_count == UINT_MAX)
		temp_count = 0;
	num_str = ft_itoa(temp_count++);
	if (!num_str)
		return (NULL);
	temp_name = ft_strjoin("/tmp/heredoc_", num_str);
	free(num_str);
	return (temp_name);
}

int	open_temp_file(const char *temp_file)
{
	int	fd;

	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("Error opening file");
		return (-1);
	}
	return (fd);
}
