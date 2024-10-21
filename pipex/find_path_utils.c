/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:14:52 by ssukhova          #+#    #+#             */
/*   Updated: 2024/07/24 19:11:07 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**split_command(char *cmd)
{
	return (ft_split(cmd, ' '));
}

char	*get_path_variable(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL && ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	if (envp[i] == NULL)
	{
		ft_printf("PATH not found in environment variables\n");
		return (NULL);
	}
	return (envp[i] + ft_strlen("PATH="));
}

char	**split_path_variable(char *path_var)
{
	return (ft_split(path_var, ':'));
}

char	*find_executable(char *cmd, char **paths)
{
	char	*part_path;
	char	*path;
	int		i;

	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

void	free_path_data(t_path_data *data)
{
	int	i;

	if (data->paths)
	{
		i = 0;
		while (data->paths[i])
			free(data->paths[i++]);
		free(data->paths);
	}
	if (data->args)
	{
		i = 0;
		while (data->args[i])
			free(data->args[i++]);
		free(data->args);
	}
}
