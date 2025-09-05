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

static int	count_pipeline_nodes(t_node *node)
{
	if (!node)
		return 0;
	if (node->type == OPERATOR && node->u_data.op.type == PIPE)
		return count_pipeline_nodes(node->u_data.op.left) + count_pipeline_nodes(node->u_data.op.right);
	return 1;
}

static void	collect_pipeline_nodes(t_node *node, t_node **arr, int *idx)
{
	if (!node)
		return;
	if (node->type == OPERATOR && node->u_data.op.type == PIPE)
	{
		collect_pipeline_nodes(node->u_data.op.left, arr, idx);
		collect_pipeline_nodes(node->u_data.op.right, arr, idx);
		return;
	}
	arr[(*idx)++] = node;
}

bool	has_redir_out(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == RED_OUT || redir->type == RED_APPEND)
			return true;
		redir = redir->next;
	}
	return false;
}

bool	has_redir_in(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == RED_IN || redir->type == RED_HERE_DOC)
			return true;
		redir = redir->next;
	}
	return false;
}

int	execute_pipeline(t_program *program, t_node *node)
{
	int	n_cmds;
	t_node	**cmds = NULL;
	int	(*pipes)[2] = NULL;
	pid_t	*pids = NULL;
	int	i;
	int	status = 0;

	t_program *prog = program;

	t_node *root = node;
	t_node *n;

	// Count commands in pipeline
	t_node *tmp = root;
	t_node *left;
	(void)tmp;
	n_cmds = count_pipeline_nodes(root);
	if (n_cmds <= 0)
		return 0;

	// Allocate arrays
	cmds = malloc(sizeof(t_node *) * n_cmds);
	if (!cmds)
		return 1;
	for (i = 0; i < n_cmds; i++)
		cmds[i] = NULL;

	i = 0;
	collect_pipeline_nodes(root, cmds, &i);

	// Create pipes (n_cmds - 1)
	if (n_cmds > 1)
	{
		pipes = malloc(sizeof(int[2]) * (n_cmds - 1));
		if (!pipes)
		{
			free(cmds);
			return 1;
		}
		for (i = 0; i < n_cmds - 1; i++)
		{
			if (pipe(pipes[i]) == -1)
			{
				perror("pipe");
				// cleanup previously created pipes
				for (int j = 0; j < i; j++)
				{
					close(pipes[j][0]);
					close(pipes[j][1]);
				}
				free(pipes);
				free(cmds);
				return 1;
			}
		}
	}

	pids = malloc(sizeof(pid_t) * n_cmds);
	if (!pids)
	{
		if (pipes)
			free(pipes);
		free(cmds);
		return 1;
	}

	// Fork each command
	for (i = 0; i < n_cmds; i++)
	{
		pid_t pid = fork();
		if (pid == -1)
		{
			perror("fork");
			// continue to cleanup
			pids[i] = -1;
			continue;
		}

		if (pid == 0)
		{
			// Child
			t_cmd_data *cmd = &cmds[i]->u_data.cmd;

			// Initialize pipefd fields
			cmd->pipefd[0] = -1;
			cmd->pipefd[1] = -1;

			// If has left pipe and no input redir, set pipe read end
			if (i > 0)
			{
				if (!has_redir_in(cmd->redir))
					cmd->pipefd[0] = pipes[i - 1][0];
			}
			// If has right pipe and no output redir, set pipe write end
			if (i < n_cmds - 1)
			{
				if (!has_redir_out(cmd->redir))
					cmd->pipefd[1] = pipes[i][1];
			}

			// Close unused pipe fds in child: close all except those assigned
			if (pipes)
			{
				for (int j = 0; j < n_cmds - 1; j++)
				{
					if (cmd->pipefd[0] != pipes[j][0])
						close(pipes[j][0]);
					if (cmd->pipefd[1] != pipes[j][1])
						close(pipes[j][1]);
				}
			}

			// Process redirections (opens files / heredocs)
			if (cmd->redir && process_redir(cmd, prog) != 0)
				exit(1);

			// Setup final fds (dup2 pipe fds or file fds)
			set_final_fds(cmd);

			// Execute builtin or external
			if (is_builtin(cmds[i]->u_data.cmd.argv[0]))
				exit(execute_builtin(prog, cmds[i], true));
			else
			{
				exec_cmd_inchild(cmds[i]);
				// Should not return
				exit(EXIT_FAILURE);
			}
		}

		// Parent
		pids[i] = pid;
	}

	// Parent closes all pipe fds
	if (pipes)
	{
		for (int j = 0; j < n_cmds - 1; j++)
		{
			close(pipes[j][0]);
			close(pipes[j][1]);
		}
	}

	// Wait for all children and capture last exit status
	for (i = 0; i < n_cmds; i++)
	{
		if (pids[i] <= 0)
			continue;
		int st = 0;
		waitpid(pids[i], &st, 0);
		if (WIFEXITED(st))
			status = WEXITSTATUS(st);
		else if (WIFSIGNALED(st))
			status = 128 + WTERMSIG(st);
	}

	// Cleanup
	free(pids);
	if (pipes)
		free(pipes);
	free(cmds);

	return status;
}
