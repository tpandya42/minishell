/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_00.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 12:41:23 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/08 12:41:24 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_redir_out(t_redir *redir);
bool	has_redir_in(t_redir *redir);

int	count_pipeline_nodes(t_node *node)
{
	if (!node)
		return (0);
	if (node->type == OPERATOR && node->u_data.op.type == PIPE)
		return (count_pipeline_nodes(node->u_data.op.left)
			+ count_pipeline_nodes(node->u_data.op.right));
	return (1);
}

void	collect_pipeline_nodes(t_node *node, t_node **arr, int *idx)
{
	if (!node)
		return ;
	if (node->type == OPERATOR && node->u_data.op.type == PIPE)
	{
		collect_pipeline_nodes(node->u_data.op.left, arr, idx);
		collect_pipeline_nodes(node->u_data.op.right, arr, idx);
		return ;
	}
	arr[(*idx)++] = node;
}

bool	has_redir_out(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == RED_OUT || redir->type == RED_APPEND)
			return (true);
		redir = redir->next;
	}
	return (false);
}

bool	has_redir_in(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == RED_IN || redir->type == RED_HERE_DOC)
			return (true);
		redir = redir->next;
	}
	return (false);
}

int	open_pipes(int (*pipes)[2], int n)
{
	int	i;
	int	j;

	i = 0;
	while (i < n)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			j = 0;
			while (j < i)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
				j++;
			}
			return (-1);
		}
		i++;
	}
	return (0);
}
