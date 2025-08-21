/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:30:35 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/19 16:05:47 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_find_source(char *buf, char c, char *lefto)
{
	char	*join;
	int		i;

	if (!buf)
		return (NULL);
	if (lefto == NULL)
		join = ft_strjoin("", buf);
	else
		join = ft_strjoin(lefto, buf);
	if (!join)
		return (NULL);
	if (BUFFER_SIZE > 1 && !ft_strchr(join, c))
		return (join);
	i = 0;
	while (i < BUFFER_SIZE + 1)
		buf[i++] = '\0';
	return (join);
}

static char	*ft_handle_eof(char **lefto, char *buf, char c)
{
	char	*line;
	char	*tmp;

	line = NULL;
	if (*lefto && **lefto && ft_strchr(*lefto, c) != NULL)
	{
		line = ft_process_str(*lefto, ft_strchr(*lefto, c), 0);
		tmp = ft_process_str(*lefto, ft_strchr(*lefto, c), 1);
		free (*lefto);
		*lefto = tmp;
		return (free(buf), line);
	}
	if (*lefto && **lefto && gnl_ft_strchr(*lefto, c) == NULL)
		return (line = *lefto, free (buf), *lefto = NULL, line);
	return (free (buf), line);
}

static int	handle_leftover_line(char **lefto, char *join, char c, char **line)
{
	*line = NULL;
	if (ft_strchr(join, c) == NULL)
	{
		free(*lefto);
		*lefto = join;
		return (0);
	}
	else
	{
		free(*lefto);
		*lefto = ft_process_str(join, gnl_ft_strchr(join, c), 1);
		*line = ft_process_str(join, gnl_ft_strchr(join, c), 0);
		free(join);
		return (1);
	}
}

static char	*read_find(int fd, char *buf, char **lefto)
{
	char	c;
	char	*join;
	char	*line;
	ssize_t	by_read;

	c = '\n';
	line = NULL;
	by_read = 1;
	while (by_read > 0)
	{
		by_read = read(fd, buf, BUFFER_SIZE);
		if (by_read < 0)
			return (free(buf), free(*lefto), *lefto = NULL, NULL);
		buf[by_read] = '\0';
		if (by_read == 0)
			return (ft_handle_eof (lefto, buf, c));
		join = ft_find_source(buf, c, *lefto);
		if (!join)
			return (free(buf), free(*lefto), *lefto = NULL, NULL);
		if (handle_leftover_line(lefto, join, c, &line))
			break ;
	}
	return (free(buf), line);
}

char	*get_next_line(int fd)
{
	static char	*lefto;
	char		*buf;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buf)
		return (NULL);
	line = read_find(fd, buf, &lefto);
	if (!line && lefto && *lefto == '\0')
	{
		free(lefto);
		lefto = NULL;
	}
	return (line);
}
