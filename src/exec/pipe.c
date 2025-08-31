/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:59:38 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/23 12:01:14 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//uses pipdefd[1] to write
pid_t	execute_left(t_program *program, t_node *left_node, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Error: Fork failed for left cmd");
		return (-1);
	}
	else if (pid == 0)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("Error: dup2 failed for left cmd");
			exit(1);
		}
		close_fd(&pipefd[0]);
		close_fd(&pipefd[1]);
		execution(program, left_node, true);
		exit(EXIT_FAILURE);
	}
	return (pid);
}

pid_t	execute_right(t_program *program, t_node *right_node, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Error: Fork failed for right cmd");
		return (-1);
	}
	else if (pid == 0)
	{
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("Error: dup2 failed for right cmd");
			exit(1);
		}
		close_fd(&pipefd[0]);
		close_fd(&pipefd[1]);
		execution(program, right_node, true);
		exit(EXIT_FAILURE);
	}
	return (pid);
}

int	close_all_pipefd(int *pipefd_in, int *pipefd_out)
{
	close_fd(pipefd_in);
	close_fd(pipefd_out);
	return (1);
}

//this consider left and right child
//in waitpid 0 makes wait child
//pids[0] is left_pid
//pids[1] is right pid
int	execute_pipeline(t_program *program, t_node *node)
{
	pid_t	pids[2];
	int		pipefd[2];
	int		status;

	if (pipe(pipefd) == -1)
	{
		perror("Error: Pipe failed");
		return (1);
	}
	pids[0] = execute_left(program, node->u_data.op.left, pipefd);
	if (pids[0] == -1)
	{
		close_all_pipefd(&pipefd[0], &pipefd[1]);
		return (1);
	}
	close_fd(&pipefd[1]);
	pids[1] = execute_right(program, node->u_data.op.right, pipefd);
	if (pids[1] == -1)
	{
		close_fd(&pipefd[0]);
		waitpid(pids[0], NULL, 0);
		return (1);
	}
	close_fd(&pipefd[0]);
	status = wait_children(pids[0], pids[1], &status);
	restore_std(program);
	return (status);
}
