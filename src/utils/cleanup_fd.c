/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_fd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 15:42:30 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/11 18:02:09 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//This function is a low-level utility that 
//closes a command's specific file descriptors,
//such as those used for redirections.
void	cleanup_fds(t_cmd_data *cmd)
{
	if (cmd->fd_in != STDIN_FILENO && cmd->fd_in >= 0)
		close_fd(&cmd->fd_in);
	if (cmd->fd_out != STDOUT_FILENO && cmd->fd_out >= 0)
		close_fd(&cmd->fd_out);
}

//This is a high-level function that directs
//the cleanup process based on the node's type
//in the abstract syntax tree (AST).
int	cleanup_fd(t_node *node, t_nodetype type)
{
	if (!node)
		return (-1);
	if (type == COMMAND)
		cleanup_cmd_node(node);
	else if (type == OPERATOR)
		cleanup_operator_fd(node);
	return (0);
}

int	cleanup_cmd_node(t_node *node)
{
	t_cmd_data	*cmd;

	if (!node || node->type != COMMAND)
		return (-1);
	cmd = &node->u_data.cmd;
	if (cmd->fd_in != STDIN_FILENO)
		close_fd(&node->u_data.cmd.fd_in);
	if (cmd->fd_out != STDOUT_FILENO)
		close_fd(&node->u_data.cmd.fd_out);
	return (0);
}

int	cleanup_operator_fd(t_node *node)
{
	if (!node)
		return (-1);
	if (node->type == OPERATOR)
	{
		if (node->u_data.op.left)
			cleanup_operator_fd(node->u_data.op.left);
		if (node->u_data.op.right)
			cleanup_operator_fd(node->u_data.op.right);
	}
	else if (node->type == COMMAND)
		cleanup_cmd_node(node);
	return (0);
}
