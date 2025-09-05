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

/**
*   @brief sets up I/O redi and executes cmd
*
*   1. Call set-up redirections
*   2. Call exec_external_cmd
*   3. Handles if execution fails
*   4. @return exit
*
*   - use: only within forked childs
*
*   @note v0: execute single external cmd: no pipes no builtins
*/
void	child_process(t_program *program, t_node *node)
{
	t_cmd_data		*cmd;
	int				status;

	cmd = &node->u_data.cmd;
	
	// Debug output removed
	
	// Setup redirections here (already processed in handle_cmd_exec)
	if (setup_redir(cmd) != 0)
		exit(EXIT_FAILURE);
	
	if (is_builtin(cmd->argv[0]))
	{
		status = execute_builtin(program, node, true);
		exit(status);
	}
	else
	{
		exec_cmd_inchild(node);//this was to execute only single external cmd
		perror (BOLD RED "Exec/Builtin failed" RESET);
		// cleanup_fd(node, node->type);// Only cleanup FDs opened by this child. CHECK IF W OTHERS
		exit(EXIT_FAILURE);
	}
}
