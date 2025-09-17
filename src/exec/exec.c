/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:32:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/12 14:49:33 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd_inchild(t_node *node)
{
	char	*cmd_path;

	cmd_path = find_path(node->u_data.cmd.argv[0]);
	if (!cmd_path)
	{
		ft_print_error(node->u_data.cmd.argv[0]);
		ft_print_error(": command not found\n");
		exit(127);
	}
	execve(cmd_path, node->u_data.cmd.argv, node->u_data.cmd.env);
	ft_print_error(node->u_data.cmd.argv[0]);
	ft_print_error(": cannot execute\n"); 
	free(cmd_path);
	exit(126);
}

int	is_operator_str(const char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, "|") == 0 || ft_strcmp(str, "&&") == 0 || ft_strcmp(str,
			"||") == 0 || ft_strcmp(str, ";") == 0)
		return (1);
	return (0);
}

void	set_final_fds(t_cmd_data *cmd)
{
	set_pipe_input(cmd);
	set_input_redir(cmd);
	set_pipe_output(cmd);
	set_output_redir(cmd);
	if (cmd->pipefd[0] >= 0)
		close_fd(&cmd->pipefd[0]);
	if (cmd->pipefd[1] >= 0)
		close_fd(&cmd->pipefd[1]);
}

int	handle_cmd_exec(t_program *program, t_node *node, bool is_pipe_child)
{
	t_cmd_data	*cmd;
	char		*name;
	char		*unq;

	if (!node || !node->u_data.cmd.argv)
		return (1);
	cmd = &node->u_data.cmd;
	unq = strip_outer_quotes(cmd->argv[0]);
	if (unq != NULL)
	{
		free(cmd->argv[0]);
		cmd->argv[0] = unq;
	}
	name = cmd->argv[0];
	if (is_operator_str(name))
	{
		ft_print_syntax_error(name);
		return (2);
	}
	if (is_pipe_child)
		child_exec_flow(program, node, cmd);
	else if (is_builtin(name))
		return (handle_builtin_no_pipe(program, node, cmd));
	return (external_no_pipe(program, node, cmd));
}

int	execution(t_program *program, t_node *node, bool is_pipe_child)
{
	int	status;

	if (!node)
	{
		program->last_exit_status = 0;
		return (0);
	}
	if (node->type == COMMAND)
		status = handle_cmd_exec(program, node, is_pipe_child);
	else if (node->type == OPERATOR)
		status = handle_operator(program, node, is_pipe_child);
	else
		status = 1;
	program->last_exit_status = status;
	return (status);
}
