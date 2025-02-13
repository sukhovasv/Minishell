/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhova <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:55:50 by ssukhova          #+#    #+#             */
/*   Updated: 2024/07/16 12:55:52 by ssukhova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(const char *cmd)
{
    return (ft_strncmp(cmd, "cd", 2) == 0 || 
            ft_strncmp(cmd, "pwd", 3) == 0 || 
            ft_strncmp(cmd, "echo", 4) == 0 ||
            ft_strncmp(cmd, "exit", 4) == 0 || 
            ft_strncmp(cmd, "export", 6) == 0 || 
            ft_strncmp(cmd, "unset", 5) == 0 ||
            ft_strncmp(cmd, "env", 3) == 0);
}

int execute_builtin(char **argv, t_env *env)
{
    if (ft_strncmp(argv[0], "cd", 2) == 0)
        return (builtin_cd(argv));
    else if (ft_strncmp(argv[0], "pwd", 3) == 0)
        return (builtin_pwd());
    else if (ft_strncmp(argv[0], "echo", 4) == 0)
        return (builtin_echo(argv));
    else if (ft_strncmp(argv[0], "export", 6) == 0)
        return (builtin_export(argv, env));
    else if (ft_strncmp(argv[0], "unset", 5) == 0)
        return (builtin_unset(argv, env));
    else if (ft_strncmp(argv[0], "env", 3) == 0)
        return (builtin_env(env->environ));
    else if (ft_strncmp(argv[0], "exit", 4) == 0)
        return (builtin_exit(argv));
    return (1);
}
