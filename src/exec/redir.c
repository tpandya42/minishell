/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:59:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/22 18:19:01 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Redirects STDIN_FILENO to a specified fd
*/
static int	redir_in(int fd)
{
	int	result;

	result = dup2(fd, STDIN_FILENO);
	if (result == -1)
	// {
		perror ("Dup2 failed for redir_in");
		// close_fd (fd);//check if is ok onlye closing in setup-redir
	// }
	// close_fd(fd);//check if is ok onlye closing in setup-redir
	return (result);
}

/**
* @brief Redirects STDOUT_FILENO to a specified fd
*/
static int	redir_out(int fd)
{
	int	result;

	result = dup2(fd, STDOUT_FILENO);
	if (result == -1)
	// {
		perror ("Dup2 failed for redir_out");
		// close_fd(fd);//check if is ok only closing in setup-redir
		// return (1);
	// }
	// close_fd(fd);//check if is ok only closing in setup-redir
	return (result);
}

/**
* @brief S to apply the final file descriptors to the 
* standard I/O streams using dup2
* Uses cmd->fd_in and cmd->fd_out, closes the temporary fds
* after duplicating them to STDIN_FILENO and STDOUT_FILENO.
*/
int	setup_redir(t_cmd_data *cmd)
{
	int	cmd_fd_in;
	int	cmd_fd_out;

	cmd_fd_in = cmd->fd_in;
	cmd_fd_out = cmd ->fd_out;
	if (cmd_fd_in != -1)
	{
		if (redir_in(cmd_fd_in) == -1)
		{
			close_fd(&cmd_fd_in);
			return (perror("Failed redir_input: "), 1);
		}
		close_fd(&cmd_fd_in);
	}
	if (cmd_fd_out != -1)
	{
		if (redir_out(cmd_fd_out) == -1)
		{
			close_fd(&cmd_fd_out);
			return (perror("Failed redir_out: "), 1);
		}
		close_fd(&cmd_fd_out);
	}
	return (0);
}

// To update the command's file descriptors.
static void	update_redir_fd(int new_fd, int *cmd_fd)
{
	if (*cmd_fd != STDIN_FILENO && *cmd_fd != STDOUT_FILENO)
		close_fd(cmd_fd);
	*cmd_fd = new_fd;
}

//iterates over the command’s linked list of redirections
//to open target files and set cmd_fdin and cmd_fdout 
//after all redir
// 1. Open the file and check for errors immediately.
// If open fails, close all previously opened FDs.
// 3. Close the previous output FD if it's not STDOUT.
int	process_redir(t_cmd_data *cmd)
{
	t_redir	*current_redir;

	current_redir = cmd->redir;
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	while (current_redir)
	{
		if (open_redir_filename(current_redir) != 0)
		{
			if (cmd->fd_in != STDIN_FILENO)
				close_fd(&cmd->fd_in);
			if (cmd->fd_out != STDOUT_FILENO)
				close_fd(&cmd->fd_out);
			return (1);
		}
		if (current_redir->type == RED_IN)
			update_redir_fd(current_redir->fd, &cmd->fd_in);
		else
			update_redir_fd(current_redir->fd, &cmd->fd_out);
		current_redir = current_redir->next;
	}
	return (0);
}

