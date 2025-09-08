/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 12:07:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/26 15:32:08 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_process(t_program *program, t_node *node)
{
	t_cmd_data	*cmd;
	int			status;

	cmd = &node->u_data.cmd;
	if (setup_redir(cmd) != 0)
		exit(EXIT_FAILURE);
	if (is_builtin(cmd->argv[0]))
	{
		status = execute_builtin(program, node, true);
		exit(status);
	}
	else
	{
		exec_cmd_inchild(node);
		perror(BOLD RED "Exec/Builtin failed" RESET);
		exit(EXIT_FAILURE);
	}
}
