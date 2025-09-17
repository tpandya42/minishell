/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_00.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:57:51 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/12 14:39:27 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_operator(t_program *program, t_node *node, bool is_pipe_child)
{
	int	left_status;

	(void)is_pipe_child;
	if (node->u_data.op.type == PIPE)
		return (execute_pipefile(program, node));
	else if (node->u_data.op.type == AND)
	{
		left_status = execution(program, node->u_data.op.left, true);
		if (left_status == 0)
			return (execution(program, node->u_data.op.right, true));
		return (left_status);
	}
	return (1);
}

void	set_pipe_input(t_cmd_data *cmd)
{
	if (cmd->fd_in == STDIN_FILENO && cmd->pipefd[0] >= 0)
	{
		if (dup2(cmd->pipefd[0], STDIN_FILENO) == -1)
			exit(1);
		close_fd(&cmd->pipefd[0]);
	}
}

void	set_input_redir(t_cmd_data *cmd)
{
	if (cmd->fd_in >= 0 && cmd->fd_in != STDIN_FILENO)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
			exit(1);
		close_fd(&cmd->fd_in);
	}
}

void	set_pipe_output(t_cmd_data *cmd)
{
	if (cmd->fd_out == STDOUT_FILENO && cmd->pipefd[1] >= 0)
	{
		if (dup2(cmd->pipefd[1], STDOUT_FILENO) == -1)
			exit(1);
		close_fd(&cmd->pipefd[1]);
	}
}

void	set_output_redir(t_cmd_data *cmd)
{
	if (cmd->fd_out >= 0 && cmd->fd_out != STDOUT_FILENO)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
			exit(1);
		close_fd(&cmd->fd_out);
	}
}
