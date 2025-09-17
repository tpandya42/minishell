/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:22:09 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/12 14:55:39 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir_token(t_token *tok)
{
	return (tok && (tok->type == REDIR_IN || tok->type == REDIR_OUT
			|| tok->type == APPEND || tok->type == HEREDOC));
}

int	redir_in(int fd)
{
	if (fd >= 0)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			return (1);
	}
	return (0);
}

int	redir_out(int fd)
{
	if (fd >= 0)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (1);
	}
	return (0);
}

int	setup_redir(t_cmd_data *cmd)
{
	if (cmd->fd_in != STDIN_FILENO && cmd->fd_in >= 0)
	{
		if (redir_in(cmd->fd_in) != 0)
			return (1);
	}
	else if (cmd->pipefd[0] >= 0)
	{
		if (redir_in(cmd->pipefd[0]) != 0)
			return (1);
		close_fd(&cmd->pipefd[0]);
	}
	if (cmd->fd_out != STDOUT_FILENO && cmd->fd_out >= 0)
	{
		if (redir_out(cmd->fd_out) != 0)
			return (1);
	}
	else if (cmd->pipefd[1] >= 0)
	{
		if (redir_out(cmd->pipefd[1]) != 0)
			return (1);
		close_fd(&cmd->pipefd[1]);
	}
	return (0);
}
