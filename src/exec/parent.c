/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:15:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/15 07:17:53 by albetanc         ###   ########.fr       */
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

int	exec_cmd_nopipe(t_program *program, t_node *node)
{
	pid_t	pid;
	int		child_status;

	pid = fork();
	if (pid == -1)
	{
		perror("Fork");
		return (1);
	}
	else if (pid == 0)
	{
		set_signal_child_process();
		child_process(program, node);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &child_status, 0);
		if (WIFEXITED(child_status))
			return (WEXITSTATUS(child_status));
		else if (WIFSIGNALED(child_status))
			return (128 + WTERMSIG(child_status));
		return (1); 
	}
}
