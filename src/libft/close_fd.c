/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:53:41 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/12 14:12:55 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
*   Closes a given file descriptor if valid and marks it as closed.
*   1. return 0 if there is nothing to close
*/
int	close_fd(int *fd)
{
	if (!fd || *fd < 0)
		return (-1);
	if (*fd == STDIN_FILENO 
		|| *fd == STDOUT_FILENO || *fd == STDERR_FILENO)
		return (0); 
	if (close(*fd) == -1)
		return (1);
	*fd = -1;
	return (0);
}
