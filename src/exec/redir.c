/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:59:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/05 08:36:43 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_redir_fd(int new_fd, int *cmd_fd)
{
	if (new_fd == -1)
		return ;
	if (*cmd_fd != STDIN_FILENO && *cmd_fd != STDOUT_FILENO)
		close_fd(cmd_fd);
	*cmd_fd = new_fd;
}

static int	handle_heredoc(t_redir *r, t_program *program, int *fd_in)
{
	if (heredoc_prepare(r, program) != 0)
	{
		if (g_signal_value == SIGINT)
			program->last_exit_status = 130;
		return (1);
	}
	update_redir_fd(r->fd, fd_in);
	return (0);
}

static int	handle_file_redir(t_redir *r, int *fd_in, int *fd_out)
{
	if (open_redir_filename(r) != 0)
		return (1);
	if (r->type == RED_IN)
		update_redir_fd(r->fd, fd_in);
	else
		update_redir_fd(r->fd, fd_out);
	return (0);
}

int	process_redir(t_cmd_data *cmd, t_program *program)
{
	t_redir	*r;

	r = cmd->redir;
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	while (r)
	{
		if (r->type == RED_HERE_DOC)
		{
			if (handle_heredoc(r, program, &cmd->fd_in))
				return (1);
		}
		else if (handle_file_redir(r, &cmd->fd_in, &cmd->fd_out))
			return (1);
		r = r->next;
	}
	return (0);
}
