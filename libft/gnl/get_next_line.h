/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 15:29:36 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/19 16:05:23 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <stdlib.h>
# include <unistd.h>
# include <stddef.h>
# include "../libft.h"

char	*get_next_line(int fd);
size_t	ft_str_utils(char *dst, const char *src, size_t dstsize, int flag);
char	*gnl_ft_strchr(const char *str, int c);
size_t	ft_strlcat(char *dst, const char *src, size_t dst_size);
char	*ft_process_str(char *join, char *c, int flag);

#endif
