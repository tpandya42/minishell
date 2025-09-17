/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_01.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 12:41:38 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/08 12:41:41 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_all_pipes(int (*pipes)[2], int n)
{
	int	j;

	j = 0;
	while (j < n)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}

void	child_process_pipe(t_node *node, t_pipe_ctx *ctx, t_program *prog)
{
	t_cmd_data	*cmd;

	cmd = &node->u_data.cmd;
	init_cmd_pipefds(cmd, ctx->idx, ctx->n_cmds, ctx->pipes);
	close_unused_pipes(cmd, ctx->n_cmds, ctx->pipes);
	perform_exec(node, prog, cmd);
}

int	wait_children_pipe(pid_t *pids, int n_cmds)
{
	int	status;
	int	st;
	int	i;

	status = 0;
	i = 0;
	while (i < n_cmds)
	{
		if (pids[i] <= 0)
			continue ;
		st = 0;
		waitpid(pids[i], &st, 0);
		if (WIFEXITED(st))
			status = WEXITSTATUS(st);
		else if (WIFSIGNALED(st))
			status = 128 + WTERMSIG(st);
		i++;
	}
	return (status);
}

t_node	**alloc_cmds(t_node *root, int n_cmds)
{
	t_node	**cmds;
	int		i;
	int		j;

	j = 0;
	cmds = malloc(sizeof(t_node *) * n_cmds);
	i = 0;
	if (!cmds)
		return (NULL);
	while (j < n_cmds)
		cmds[j++] = NULL;
	collect_pipeline_nodes(root, cmds, &i);
	return (cmds);
}

int	(*alloc_pipes_and_open(int n_pipes))[2]
{
	int	(*pipes)[2];

	pipes = malloc (sizeof (int [2]) * n_pipes);
	if (!pipes)
		return (NULL);
	if (open_pipes(pipes, n_pipes) == -1)
	{
		free(pipes);
		return (NULL);
	}
	return (pipes);
}
