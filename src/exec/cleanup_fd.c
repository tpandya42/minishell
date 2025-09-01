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

#include "minishell.h"
#include "cleanup_fd.h"

// Close any open file descriptors in the command
void cleanup_fds(t_cmd_data *cmd)
{
    // Flush stdout before closing
    fflush(stdout);
    
    // Close input file descriptor if open
    if (cmd->fd_in != STDIN_FILENO && cmd->fd_in >= 0)
    {
        fprintf(stderr, "DEBUG: Closing fd_in=%d\n", cmd->fd_in);
        close_fd(&cmd->fd_in);
    }
    
    // Close output file descriptor if open
    if (cmd->fd_out != STDOUT_FILENO && cmd->fd_out >= 0)
    {
        fprintf(stderr, "DEBUG: Closing fd_out=%d\n", cmd->fd_out);
        close_fd(&cmd->fd_out);
    }
}
