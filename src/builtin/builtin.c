/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:10:28 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/26 14:42:37 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(const char *cmd_name)
{
	if (ft_strcmp(cmd_name, "echo") == 0)
		return (BUILTIN_ECHO);
	if (ft_strcmp(cmd_name, "cd") == 0)
		return (BUILTIN_CD);
	if (ft_strcmp(cmd_name, "pwd") == 0)
		return (BUILTIN_PWD);
	if (ft_strcmp(cmd_name, "export") == 0)
		return (BUILTIN_EXPORT);
	if (ft_strcmp(cmd_name, "unset") == 0)
		return (BUILTIN_UNSET);
	if (ft_strcmp(cmd_name, "env") == 0)
		return (BUILTIN_ENV);
	if (ft_strcmp(cmd_name, "exit") == 0)
		return (BUILTIN_EXIT);
	return (BUILTIN_NONE);
}

static int	dispatch_builtin(t_program *program,
		t_node *node, t_builtin_type type)
{
	if (type == BUILTIN_ECHO)
		return (my_echo(program, node));
	else if (type == BUILTIN_ENV)
		return (my_env(program, node));
	else if (type == BUILTIN_CD)
		return (my_cd(program, node));
	else if (type == BUILTIN_PWD)
		return (my_pwd(program, node));
	else if (type == BUILTIN_EXPORT)
		return (my_export(program, node));
	else if (type == BUILTIN_UNSET)
		return (my_unset(program, node));
	else if (type == BUILTIN_EXIT)
		my_exit(program, node);
	return (1);
}

int	execute_builtin(t_program *program, t_node *node, bool is_pipe_child)
{
	t_cmd_data		*cmd;
	t_builtin_type	type;

	(void)is_pipe_child;
	cmd = &node->u_data.cmd;
	type = is_builtin(cmd->argv[0]);
	return (dispatch_builtin(program, node, type));
}
