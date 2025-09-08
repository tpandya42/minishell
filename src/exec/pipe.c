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

static void	child_spawn_loop(t_node **cmds, int n_cmds, int (**pipes)[2],
		pid_t *pids, t_program *prog)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < n_cmds)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			pids[i] = -1;
		}
		else if (pid == 0)
			child_process_pipe(cmds[i], i, n_cmds, *pipes, prog);
		else
			pids[i] = pid;
		i++;
	}
}

static int	alloc_resources(t_node *node, int n_cmds, t_node ***cmds_out,
		int (**pipes_out)[2])
{
	t_node	**cmds;
	int	(*pipes)[2];

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

static void	free_resources(t_node **cmds, int (*pipes)[2], pid_t *pids)
{
	if (pids)
		free(pids);
	if (pipes)
		free(pipes);
	if (cmds)
		free(cmds);
}

int	execute_pipefile(t_program *program, t_node *node)
{
	int		n_cmds;
	t_node	**cmds;
	pid_t	*pids;
	int		status;
	int	(*pipes)[2];

	cmds = NULL;
	pids = NULL;
	pipes = NULL;
	n_cmds = count_pipeline_nodes(node);
	if (n_cmds <= 0)
		return (0);
	if (alloc_resources(node, n_cmds, &cmds, &pipes) == -1)
		return (1);
	if (alloc_pids(&pids, n_cmds, cmds, &pipes) == -1)
		return (1);
	child_spawn_loop(cmds, n_cmds, &pipes, pids, program);
	if (pipes)
		close_all_pipes(pipes, n_cmds - 1);
	status = wait_children_pipe(pids, n_cmds);
	free_resources(cmds, pipes, pids);
	return (status);
}
