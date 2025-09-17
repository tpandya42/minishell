/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 08:24:38 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/15 17:03:08 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

void	free_redirs(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->target)
			free(redir->target);
		free(redir);
		redir = tmp;
	}
}

void	free_cmd_arg(t_node *node)
{
	int	i;

	i = 0;
	while (node->u_data.cmd.argv[i])
	{
		free(node->u_data.cmd.argv[i]);
		i++;
	}
	free(node->u_data.cmd.argv);
	node->u_data.cmd.argv = NULL;
}

void	free_node(t_node *node)
{
	if (!node)
		return ;
	if (node->type == OPERATOR)
	{
		free_node(node->u_data.op.left);
		free_node(node->u_data.op.right);
	}
	else if (node->type == COMMAND)
	{
		cleanup_cmd_node(node);
		if (node->u_data.cmd.tokens)
		{
			free_token(node->u_data.cmd.tokens);
			node->u_data.cmd.tokens = NULL;
		}
		if (node->u_data.cmd.argv)
			free_cmd_arg(node);
		if (node->u_data.cmd.redir)
		{
			free_redirs(node->u_data.cmd.redir);
			node->u_data.cmd.redir = NULL;
		}
	}
	free (node);
}
