/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:14:37 by ssukhova          #+#    #+#             */
/*   Updated: 2024/07/24 16:14:40 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path(char *cmd, char **envp)
{
	t_path_data	data;
	char		*result_path;

	data.args = split_command(cmd);
	if (!data.args)
		return (NULL);
	data.path_var = get_path_variable(envp);
	if (!data.path_var)
	{
		free(data.args);
		return (NULL);
	}
	data.paths = split_path_variable(data.path_var);
	if (!data.paths)
	{
		free(data.args);
		return (NULL);
	}
	data.path = find_executable(data.args[0], data.paths);
	result_path = data.path;
	free_path_data(&data);
	return (result_path);
}
