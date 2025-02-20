/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:23:26 by ssukhova          #+#    #+#             */
/*   Updated: 2025/02/19 13:23:27 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	try_direct_execution(char **argv, char *fullpath)
{
	int	errcode;

	errcode = -1;
	if ((*argv[0] == '/') || *argv[0] == '.')
		errcode = access(ft_strncpy(fullpath, argv[0], PATH_MAX), F_OK);
	return (errcode);
}
