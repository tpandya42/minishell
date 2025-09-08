/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_fd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 15:42:30 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/01 15:43:14 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cleanup_fd.h"
#include "minishell.h"

void	cleanup_fds(t_cmd_data *cmd)
{
	fflush(stdout);
	if (cmd->fd_in != STDIN_FILENO && cmd->fd_in >= 0)
	{
		close_fd(&cmd->fd_in);
	}
	if (cmd->fd_out != STDOUT_FILENO && cmd->fd_out >= 0)
	{
		close_fd(&cmd->fd_out);
	}
}
