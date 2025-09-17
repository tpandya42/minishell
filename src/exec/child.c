/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 12:07:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/12 14:32:12 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_process(t_program *program, t_node *node)
{
	t_cmd_data	*cmd;
	int			status;

	cmd = &node->u_data.cmd;
	if (setup_redir(cmd) != 0)
	{
		close_fd(&program->fd_in_orig);
		close_fd(&program->fd_out_orig);
		exit(EXIT_FAILURE);
	}
	if (is_builtin(cmd->argv[0]))
	{
		status = execute_builtin(program, node, true);
		close_fd(&program->fd_in_orig);
		close_fd(&program->fd_out_orig);
		exit(status);
	}
	else
	{
		close_fd(&program->fd_in_orig);
		close_fd(&program->fd_out_orig);
		exec_cmd_inchild(node);
		perror("milanshell");
		exit(EXIT_FAILURE);
	}
}
