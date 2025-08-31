/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:15:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/23 07:45:14 by albetanc         ###   ########.fr       */
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
		child_process(program, node);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &child_status, 0);
		restore_std(program); 
		if (WIFEXITED(child_status))
			return (WEXITSTATUS(child_status));
		return (1); // Default error if not exited normally check if is handle later
	}
}
