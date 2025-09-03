/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:15:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/28 12:52:45 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_children(pid_t left_pid, pid_t right_pid, int *right_status)
{
	int	left_status;

	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, right_status, 0);
	if (WIFEXITED(*right_status))
		return (WEXITSTATUS(*right_status));
	return (1);
}

//fork before execve
//to execute external cmd without pipes
//parent actions
int	exec_cmd_nopipe(t_program *program, t_node *node)
{
	pid_t	pid;
	int		child_status;

	pid = fork();
	if (pid == -1)
	{
		perror("Fork failedcmd no pipe");
		return (1);
	}
	else if (pid == 0)//child
	{
		//CHECK FD TO CLOSE
		// if (process_redir(&node->u_data.cmd, program) != 0)//new
		// 	exit(EXIT_FAILURE);//new
		child_process(program, node);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &child_status, 0);
		// restore_std has been moved to handle_cmd_exec
		
		// Handle exit status according to how process terminated
		if (WIFEXITED(child_status))
		{
			return (WEXITSTATUS(child_status));
		}
		else if (WIFSIGNALED(child_status))
		{
			// Add 128 to signal number (bash behavior)
			// SIGINT (2) becomes 130, SIGQUIT (3) becomes 131, etc.
			return (128 + WTERMSIG(child_status));
		}
		
		return (1); // Default error if not exited normally
	}
}
