/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_00.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:57:51 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/07 22:58:07 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_operator(t_program *program, t_node *node, bool is_pipe_child)
{
	int	left_status;
	int	right_status;

	if (node->u_data.op.type == PIPE)
		return (execute_pipefile(program, node));
	else if (node->u_data.op.type == AND)
	{
		left_status = execution(program, node->u_data.op.left, true);
		if (left_status == 0)
			return (execution(program, node->u_data.op.right, true));
		return (left_status);
	}
	fprintf(stderr, BOLD RED "Error: unknow type operand for execution\n" RESET);
	return (1);
}

void	set_pipe_input(t_cmd_data *cmd)
{
	if (cmd->fd_in == STDIN_FILENO && cmd->pipefd[0] >= 0)
	{
		if (dup2(cmd->pipefd[0], STDIN_FILENO) == -1)
		{
			perror("Error: dup2 failed for pipe input");
			exit(1);
		}
		close_fd(&cmd->pipefd[0]);
	}
}

void	set_input_redir(t_cmd_data *cmd)
{
	if (cmd->fd_in >= 0 && cmd->fd_in != STDIN_FILENO)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
		{
			perror("Error: dup2 failed for input redirection");
			exit(1);
		}
		close_fd(&cmd->fd_in);
	}
}

void	set_pipe_output(t_cmd_data *cmd)
{
	if (cmd->fd_out == STDOUT_FILENO && cmd->pipefd[1] >= 0)
	{
		if (dup2(cmd->pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("Error: dup2 failed for pipe output");
			exit(1);
		}
		close_fd(&cmd->pipefd[1]);
	}
}

void	set_output_redir(t_cmd_data *cmd)
{
	if (cmd->fd_out >= 0 && cmd->fd_out != STDOUT_FILENO)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		{
			perror("Error: dup2 failed for output redirection");
			exit(1);
		}
		close_fd(&cmd->fd_out);
	}
}
