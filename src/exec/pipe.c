/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:59:38 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/12 15:23:43 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_spawn_loop(t_spawn_ctx *ctx)
{
	int			i;
	pid_t		pid;
	t_pipe_ctx	pipe_ctx;

	i = 0;
	while (i < ctx->n_cmds)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			ctx->pids[i] = -1;
		}
		else if (pid == 0)
		{
			pipe_ctx.idx = i;
			pipe_ctx.n_cmds = ctx->n_cmds;
			pipe_ctx.pipes = ctx->pipes;
			child_process_pipe(ctx->cmds[i], &pipe_ctx, ctx->prog);
		}
		else
			ctx->pids[i] = pid;
		i++;
	}
}

static int	alloc_resources(t_node *node, int n_cmds, t_node ***cmds_out,
		int (**pipes_out)[2])
{
	t_node	**cmds;
	int		(*pipes)[2];

	pipes = NULL;
	cmds = alloc_cmds(node, n_cmds);
	if (!cmds)
		return (-1);
	if (n_cmds > 1)
	{
		pipes = alloc_pipes_and_open(n_cmds - 1);
		if (!pipes)
		{
			free(cmds);
			return (-1);
		}
	}
	*cmds_out = cmds;
	*pipes_out = pipes;
	return (0);
}

static int	alloc_pids(pid_t **pids_out, int n_cmds, t_node **cmds,
		int (**pipes)[2])
{
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * n_cmds);
	if (!pids)
	{
		if (*pipes)
			free(*pipes);
		free(cmds);
		return (-1);
	}
	*pids_out = pids;
	return (0);
}

static int	init_ctx(t_program *program, t_node *node,
		t_spawn_ctx *ctx, int *n_cmds)
{
	t_node	**cmds;
	pid_t	*pids;
	int		(*pipes)[2];

	*n_cmds = count_pipeline_nodes(node);
	if (*n_cmds <= 0)
		return (0);
	if (alloc_resources(node, *n_cmds, &cmds, &pipes) == -1)
		return (1);
	if (alloc_pids(&pids, *n_cmds, cmds, &pipes) == -1)
		return (1);
	ctx->cmds = cmds;
	ctx->n_cmds = *n_cmds;
	ctx->pipes = pipes;
	ctx->pids = pids;
	ctx->prog = program;
	return (-42); 
}

int	execute_pipefile(t_program *program, t_node *node)
{
	t_spawn_ctx	ctx;
	int			n_cmds;
	int			status;

	if (init_ctx(program, node, &ctx, &n_cmds) != -42)
		return (1);
	child_spawn_loop(&ctx);
	if (ctx.pipes)
		close_all_pipes(ctx.pipes, n_cmds - 1);
	status = wait_children_pipe(ctx.pids, n_cmds);
	free_resources(ctx.cmds, ctx.pipes, ctx.pids);
	return (status);
}
