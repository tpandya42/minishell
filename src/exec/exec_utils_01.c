/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_01.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:58:27 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/07 22:58:31 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_builtin_no_pipe(t_program *program, t_node *node, t_cmd_data *cmd)
{
	int	status;

	if (cmd->redir)
	{
		if (process_redir(cmd, program) == 0)
			setup_redir(cmd);
	}
	else
		setup_redir(cmd);
	status = execute_builtin(program, node, false);
	if (cmd->redir)
	{
		cleanup_fds(cmd);
		restore_std(program);
	}
	return (status);
}

void	child_exec_flow(t_program *program, t_node *node, t_cmd_data *cmd)
{
	if (cmd->redir && process_redir(cmd, program) != 0)
		exit(EXIT_FAILURE);
	set_final_fds(cmd);
	if (is_builtin(cmd->argv[0]))
		exit(execute_builtin(program, node, true));
	exec_cmd_inchild(node);
	perror("Error executing command in pipe");
	exit(EXIT_FAILURE);
}

int	external_no_pipe(t_program *program, t_node *node, t_cmd_data *cmd)
{
	int	status;

	if (process_redir(cmd, program) != 0)
		return (1);
	setup_redir(cmd);
	status = exec_cmd_nopipe(program, node);
	cleanup_fds(cmd);
	restore_std(program);
	return (status);
}
