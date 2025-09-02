/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 12:20:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/29 13:26:52 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// --- FORWARD DECLARATIONS --- //
int	cleanup_cmd_node(t_node *node);
int	cleanup_operator_fd(t_node *node);


void	free_token(t_token *token)
{
	if (token->next)
		free_token(token->next);
	free(token->txt);
	free(token);
}

void	free_redirs(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		free(redir->target); // strdup en el lexer
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
	int	i;

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

int	cleanup_fd(t_node *node, t_nodetype type)
{
	if (!node)
		return (-1);
	if (type == COMMAND)
		cleanup_cmd_node(node);
	else if (type == OPERATOR)
		cleanup_operator_fd(node);
	return (0);
}

int	cleanup_cmd_node(t_node *node)
{
	t_cmd_data	*cmd;
	int	tmp_fd_in;
	int	tmp_fd_out;

	tmp_fd_in = node->u_data.cmd.fd_in;
	tmp_fd_out = node->u_data.cmd.fd_out;
	if (!node || node->type != COMMAND)
		return (-1);
	cmd = &node->u_data.cmd;
	if (cmd->fd_in != STDIN_FILENO)
	{
		close_fd(&node->u_data.cmd.fd_in);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		close_fd(&node->u_data.cmd.fd_out);
	}
	return (0);
}

int	cleanup_operator_fd(t_node *node)
{
	if (!node)
		return (-1);
	if (node->type == OPERATOR)
	{
		if (node->u_data.op.left)
			cleanup_operator_fd(node->u_data.op.left);
		if (node->u_data.op.right)
			cleanup_operator_fd(node->u_data.op.right);
	}
	else if (node->type == COMMAND)
		cleanup_cmd_node(node);
	return (0);
}

//Cleaning after each cmd in cmd line
//free tmp resources per each cmd like root and token list
void	free_ast_tokens(t_program *program)
{
	if (program->token_list)
	{
		free_token(program->token_list);
		program->token_list = NULL;
	}
	if (program->root)
	{
		free_node(program->root);
		program->root = NULL;
	}
}

//To centralized cleanup at the end of the program
//to finish the program
void cleanup_program(t_program *program)
{
    if (!program)
        return;

    // Free AST first (this should handle argv and node content)
    if (program->root)
    {
        free_node(program->root);
        program->root = NULL;
    }

    // Free tokens only if they weren't freed by AST
    if (program->token_list)
    {
        free_token(program->token_list);
        program->token_list = NULL;
    }

    if (program->line)
    {
        free(program->line);
        program->line = NULL;
    }

    if (program->envp_cpy)
    {
        free_array(program->envp_cpy);
        program->envp_cpy = NULL;
    }
	if (program->fd_in_orig != -1)
	{
		close_fd(&program->fd_in_orig);
	}
	if (program->fd_out_orig != -1)
	{
		close_fd(&program->fd_out_orig);
	}
}

