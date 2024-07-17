/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 21:26:10 by yberezhn          #+#    #+#             */
/*   Updated: 2023/11/28 14:51:23 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
 * Read from fd in BUFFER_SIZE chunks until \n character is present
 */
char	*gnl_read(int fd, char *rem)
{
	char *const	buf = (char *)malloc(BUFFER_SIZE + 1);
	ssize_t		bytes;

	if (!buf)
		return (NULL);
	bytes = 1;
	while (!gnl_strchr(rem, '\n') && bytes != 0)
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes <= 0)
			break ;
		buf[bytes] = 0;
		rem = gnl_realloc_str(rem, buf);
	}
	free(buf);
	return (rem);
}

/*
 * Returns null-terminated string ended with \n if present
 */
char	*gnl_line(char *buf)
{
	int		len;
	char	*line;

	if (!buf)
		return (NULL);
	len = 0;
	while (buf[len] && buf[len] != '\n')
		len++;
	if (buf[len] != '\n')
		len--;
	line = gnl_strndup(buf, len + 1);
	return (line);
}

/*
 * Returns new string, starting after first found \n
 * Free old string
 */
char	*gnl_next_str(char *buf)
{
	const int	buf_len = gnl_strlen(buf);
	int			newl_index;
	char		*new_buf;

	newl_index = 0;
	new_buf = NULL;
	while (buf[newl_index] && buf[newl_index] != '\n')
		newl_index++;
	if (newl_index < buf_len - 1)
		new_buf = gnl_strndup(&buf[newl_index + 1], buf_len - newl_index);
	free(buf);
	return (new_buf);
}

char	*get_next_line(int fd)
{
	static char	*rem[4096] = {0};
	char		*res;

	if (BUFFER_SIZE <= 0 || fd < 0 || fd > SHRT_MAX)
		return (NULL);
	rem[fd] = gnl_read(fd, rem[fd]);
	if (!rem[fd])
		return (NULL);
	res = gnl_line(rem[fd]);
	rem[fd] = gnl_next_str(rem[fd]);
	return (res);
}
