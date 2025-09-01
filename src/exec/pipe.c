/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:59:38 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/29 14:29:47 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//uses pipdefd[1] to write

pid_t	execute_left(t_program *program, t_node *left_node, int *pipefd)
{
	pid_t	pid;

	pid = fork();
	fprintf(stderr, "DEBUG: LEFT command fork pid=%d\n", pid);
	
	if (pid == -1)
	{
		perror("Error: Fork failed for left cmd");
		return (-1);
	}
	else if (pid == 0)
	{
		// Child process for left command
		
		// Close read end of pipe since we're only writing
		close(pipefd[0]);
		
		// Process redirections first if there are any
		if (left_node->u_data.cmd.redir)
			process_redir(&left_node->u_data.cmd, program);
		
		// Connect stdout to write end of pipe
		fprintf(stderr, "DEBUG: Left command redirecting stdout to pipe fd=%d\n", pipefd[1]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("Error: dup2 failed for left cmd");
			exit(1);
		}
		
		// Close pipe fd after dup2
		close(pipefd[1]);
		
		// Execute the command
		if (is_builtin(left_node->u_data.cmd.argv[0]))
		{
			int status = execute_builtin(program, left_node, true);
			exit(status);
		}
		else
		{
			exec_cmd_inchild(left_node);
			// Should not reach here
			exit(EXIT_FAILURE);
		}
	}
	
	return (pid);
}

pid_t	execute_right(t_program *program, t_node *right_node, int *pipefd)
{
	pid_t	pid;

	pid = fork();
	fprintf(stderr, "DEBUG: RIGHT command fork pid=%d\n", pid);
	
	if (pid == -1)
	{
		perror("Error: Fork failed for right cmd");
		return (-1);
	}
	else if (pid == 0)
	{
		// Child process for right command
		
		// Close write end of pipe since we're only reading
		close(pipefd[1]);
		
		// Process redirections first if there are any
		if (right_node->u_data.cmd.redir)
			process_redir(&right_node->u_data.cmd, program);
		
		// Connect stdin to read end of pipe
		fprintf(stderr, "DEBUG: Right command redirecting stdin from pipe fd=%d\n", pipefd[0]);
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("Error: dup2 failed for right cmd");
			exit(1);
		}
		
		// Close pipe fd after dup2
		close(pipefd[0]);
		
		// Execute the command
		if (is_builtin(right_node->u_data.cmd.argv[0]))
		{
			int status = execute_builtin(program, right_node, true);
			exit(status);
		}
		else
		{
			exec_cmd_inchild(right_node);
			// Should not reach here
			exit(EXIT_FAILURE);
		}
	}
	
	return (pid);
}

int	close_all_pipefd(int *pipefd_in, int *pipefd_out)
{
	close_fd(pipefd_in);
	close_fd(pipefd_out);
	return (1);
}

bool	has_redir_out(t_redir *redir)//new
{
	while (redir)
	{
		if (redir->type == RED_OUT || redir->type == RED_APPEND)
			return (true);
		redir = redir->next;
	}
	return (false);
}

bool	has_redir_in(t_redir *redir)//new
{
	while (redir)
	{
		if (redir->type == RED_IN || redir->type == RED_HERE_DOC)
			return (true);
		redir = redir->next;
	}
	return (false);
}

void	assign_pipefd(t_node *node, int pipefd[2])
{
	if (!has_redir_out(node->u_data.op.left->u_data.cmd.redir))
	{
		node->u_data.op.left->u_data.cmd.pipefd[1] = pipefd[1];
		node->u_data.op.left->u_data.cmd.fd_out = STDOUT_FILENO; //let the child decide
	}
	else
		node->u_data.op.left->u_data.cmd.pipefd[1] = -1;

	// Right command: use pipe if there are not redir
	if (!has_redir_in(node->u_data.op.right->u_data.cmd.redir))
	{
		node->u_data.op.right->u_data.cmd.pipefd[0] = pipefd[0];
		node->u_data.op.right->u_data.cmd.fd_in = STDIN_FILENO; // let the child decide
	}
	else
		node->u_data.op.right->u_data.cmd.pipefd[0] = -1;
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

	// Create the pipe
	if (pipe(pipefd) == -1)
	{
		perror("Error: Pipe failed");
		return (1);
	}
	
	fprintf(stderr, "DEBUG: Created pipe: read_fd=%d, write_fd=%d\n", pipefd[0], pipefd[1]);
	
	// Execute left command (writes to pipe)
	pids[0] = execute_left(program, node->u_data.op.left, pipefd);
	if (pids[0] == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	
	// Execute right command (reads from pipe)
	pids[1] = execute_right(program, node->u_data.op.right, pipefd);
	if (pids[1] == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		waitpid(pids[0], NULL, 0);
		return (1);
	}
	
	// Parent closes both ends of the pipe
	close(pipefd[0]);
	close(pipefd[1]);
	
	// Wait for both processes to complete
	waitpid(pids[0], NULL, 0);
	waitpid(pids[1], &status, 0);
	
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else
		status = 1;
		
	fprintf(stderr, "DEBUG: Both pipe processes completed with status %d\n", status);
	
	return (status);
}
