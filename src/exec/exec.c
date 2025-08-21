/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:32:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/20 15:43:50 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Executes a cmd with execve
//doesn't fork
void	exec_cmd_inpipe(t_node *node)
{
	char	*cmd_path;

	cmd_path = find_path (node->u_data.cmd.argv[0]);
	if (!cmd_path)
	{
		perror (BOLD RED "command_path not found" RESET);
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, node->u_data.cmd.argv, node->u_data.cmd.env);
	perror("Error: execve failed");
	free(cmd_path);
	exit(EXIT_FAILURE);
}

static int	handle_operator(t_program *program, t_node *node, bool is_pipe_child)//NEW
{
	int	left_status;
	int	right_status;

	if (node->u_data.op.type == PIPE)
		return (execute_pipeline(program, node));//TODO connect with pipe.c and program
	else if (node->u_data.op.type == AND)
	{
		left_status = execution(program, node->u_data.op.left, true);
		if (left_status == 0)
			return (execution(program, node->u_data.op.right, true));
		return (left_status);
	}
	// else if (node->u_data.op.type == OR)
	// {
	// 	left_status = execution(program, node->u_data.op.left);
	// 	if (left_status != 0)
	// 		return (execution(program, node->u_data.op.right));
	// 	return (left_status);
	// }
	// else if (node->u_data.op.type == SEMICOLON)
	// {
	// 	execution(program, node->u_data.op.left);
	// 	return (execution(program, node->u_data.op.right));
	// }
	fprintf(stderr, BOLD RED "Error: unknow type operand for execution\n" RESET);
	return (1);
}

//decides cmd execution
int	handle_cmd_exec(t_program *program, t_node *node, bool is_pipe_child)//make it shorter
{
	int	status;
	char *cmd_name;

	if (!node || !node->u_data.cmd.argv)
		return (1);
	cmd_name = node->u_data.cmd.argv[0];
	if (is_pipe_child)
	{
		if (is_builtin(cmd_name))
		{
			status = execute_builtin(program, node);
			exit (status);
		}
		else
		{
			exec_cmd_inpipe(node);
			exit (1);
		}
	}
	else
	{
		if (is_builtin(cmd_name))
		{
			status = execute_builtin(program, node);
			program->last_exit_status = status;
			return (status);
		}
		else
			return (exec_cmd_nopipe(program, node));
	}
}

int	execution(t_program *program, t_node *node, bool is_pipe_child)
{
	int	status;

	fprintf(stderr, MAGENTA BOLD "About to dispatch a node execution\n" RESET);//test
	if (!node)
	{
		program->last_exit_status = 0;
		fprintf(stderr, MAGENTA BOLD "Last cmd updated: %d\n" RESET, program->last_exit_status);//test
		return (0);
	}
	if (node->type == COMMAND) 
	{
		status = handle_cmd_exec(program, node, is_pipe_child);
		fprintf(stderr, MAGENTA BOLD "Will be a cmd\n" RESET);//test
	}
	else if (node->type == OPERATOR)
	{
		status = handle_operator(program, node, is_pipe_child);
		fprintf(stderr, MAGENTA BOLD "Will be a oprator\n" RESET);//test
	}
	else
	{
		fprintf(stderr, BOLD RED "Error: unknow type node for execution\n" RESET);
		status = 1;
	}
	program->last_exit_status = status;
	return (status);
}
