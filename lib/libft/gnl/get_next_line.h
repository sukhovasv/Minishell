/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 21:26:04 by yberezhn          #+#    #+#             */
/*   Updated: 2023/11/15 21:26:07 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 80
# endif //BUFFER_SIZE

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>

char	*get_next_line(int fd);
char	*gnl_strndup(char const *src, size_t len);
int		gnl_strlen(char *str);
char	*gnl_strchr(const char *s, int c);
char	*gnl_realloc_str(char *left_str, char *buff);

#endif //GET_NEXT_LINE_H
