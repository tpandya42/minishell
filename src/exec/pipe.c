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

// Forward declarations
bool	has_redir_out(t_redir *redir);
bool	has_redir_in(t_redir *redir);

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
		
		// Store pipe write fd in command struct for later use
		if (!has_redir_out(left_node->u_data.cmd.redir))
			left_node->u_data.cmd.pipefd[1] = pipefd[1];
		else
			close(pipefd[1]); // Close pipe write end if we have explicit redirection
		
		// Process redirections first
		if (left_node->u_data.cmd.redir)
		{
			fprintf(stderr, "DEBUG: Processing redirections for left command\n");
			if (process_redir(&left_node->u_data.cmd, program) != 0)
				exit(1);
		}
		
		// Setup all redirections (including pipe)
		setup_redir(&left_node->u_data.cmd);
		
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
		
		// Store pipe read fd in command struct for later use
		if (!has_redir_in(right_node->u_data.cmd.redir))
			right_node->u_data.cmd.pipefd[0] = pipefd[0];
		else
			close(pipefd[0]); // Close pipe read end if we have explicit redirection
		
		// Process redirections first
		if (right_node->u_data.cmd.redir)
		{
			fprintf(stderr, "DEBUG: Processing redirections for right command\n");
			if (process_redir(&right_node->u_data.cmd, program) != 0)
				exit(1);
		}
		
		// Setup all redirections (including pipe)
		setup_redir(&right_node->u_data.cmd);
		
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
	
	// Check if left side is another pipe
	if (node->u_data.op.left->type == OPERATOR && node->u_data.op.left->u_data.op.type == PIPE)
	{
		fprintf(stderr, "DEBUG: Left side is another pipe, recursively executing\n");
		// Recursively handle left pipeline
		status = execute_pipeline(program, node->u_data.op.left);
		if (status != 0)
		{
			close(pipefd[0]);
			close(pipefd[1]);
			return (status);
		}
		pids[0] = 0; // No specific PID to wait for
	}
	else
	{
		// Execute left command (writes to pipe)
		pids[0] = execute_left(program, node->u_data.op.left, pipefd);
		if (pids[0] == -1)
		{
			close(pipefd[0]);
			close(pipefd[1]);
			return (1);
		}
	}
	
	// Check if right side is another pipe
	if (node->u_data.op.right->type == OPERATOR && node->u_data.op.right->u_data.op.type == PIPE)
	{
		fprintf(stderr, "DEBUG: Right side is another pipe, recursively executing\n");
		// For the right side pipe, we pass the read end of our pipe as stdin
		// and let it create its own output pipe
		
		// First, execute the leftmost command of the right pipeline with our pipe
		t_node *right_pipeline = node->u_data.op.right;
		pids[1] = execute_right(program, right_pipeline->u_data.op.left, pipefd);
		
		if (pids[1] == -1)
		{
			close(pipefd[0]);
			close(pipefd[1]);
			if (pids[0] > 0)
				waitpid(pids[0], NULL, 0);
			return (1);
		}
		
		// Parent closes pipe ends
		close(pipefd[0]);
		close(pipefd[1]);
		
		// Then recursively handle the rest of the right pipeline
		status = execute_pipeline(program, right_pipeline);
	}
	else
	{
		// Execute right command (reads from pipe)
		pids[1] = execute_right(program, node->u_data.op.right, pipefd);
		if (pids[1] == -1)
		{
			close(pipefd[0]);
			close(pipefd[1]);
			if (pids[0] > 0)
				waitpid(pids[0], NULL, 0);
			return (1);
		}
		
		// Parent closes both ends of the pipe
		close(pipefd[0]);
		close(pipefd[1]);
	}
	
	// Wait for processes to complete
	if (pids[0] > 0)
	{
		fprintf(stderr, "DEBUG: Waiting for left command pid=%d\n", pids[0]);
		waitpid(pids[0], NULL, 0);
	}
	
	if (pids[1] > 0)
	{
		fprintf(stderr, "DEBUG: Waiting for right command pid=%d\n", pids[1]);
		waitpid(pids[1], &status, 0);
		
		if (WIFEXITED(status))
		{
			status = WEXITSTATUS(status);
		}
		else if (WIFSIGNALED(status))
		{
			// For signals like Ctrl+C (SIGINT)
			status = 128 + WTERMSIG(status);
		}
		else
		{
			status = 1;  // Default error code
		}
			
		fprintf(stderr, "DEBUG: Right process completed with status %d\n", status);
	}
	
	return (status);
}
