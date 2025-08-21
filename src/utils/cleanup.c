/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 12:20:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/20 15:50:53 by albetanc         ###   ########.fr       */
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

// void	free_node(t_node *node)
// {
// 	if (!node)
// 		return ;
// 	if (node->type == OPERATOR)
// 	{
// 		free_node(node->u_data.op.left);
// 		free_node(node->u_data.op.right);
// 	}
// 	else if (node->type == COMMAND)
// 	{
// 		 if (node->u_data.cmd.argv)
// 		 	free_array(node->u_data.cmd.argv);
// 		node->u_data.cmd.argv = NULL;
// 	}
// 	free(node);
// }

void free_node(t_node *node)
{
    if (!node)
        return;

    if (node->type == OPERATOR)
    {
        free_node(node->u_data.op.left);
        free_node(node->u_data.op.right);
    }
    else if (node->type == COMMAND)
    {
        if (node->u_data.cmd.argv)
        {
            for (int i = 0; node->u_data.cmd.argv[i]; i++)
                free(node->u_data.cmd.argv[i]);
            free(node->u_data.cmd.argv);
            node->u_data.cmd.argv = NULL;
        }
    }
    free(node);
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

	if (!node || node->type != COMMAND)
		return (-1);
	cmd = &node->u_data.cmd;
	if (cmd->fd_in != STDIN_FILENO)
		close_fd(&node->u_data.cmd.fd_in);
	if (cmd->fd_out != STDOUT_FILENO)
		close_fd(&node->u_data.cmd.fd_out);
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
	fprintf(stderr, BOLD MAGENTA "Command processed and cleaned up\n" RESET); //TEST
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
}

