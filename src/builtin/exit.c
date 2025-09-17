/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 11:52:09 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/15 08:13:37 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_number(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	count_args(char **argv)
{
	int	count;

	count = 0;
	while (argv[count])
		count++;
	return (count);
}

static int	get_exit_code(char *arg, int last_status)
{
	if (!arg)
		return (last_status);
	if (!is_valid_number(arg))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		return (2);
	}
	return ((unsigned char)ft_atoi(arg));
}

void	my_exit(t_program *program, t_node *node)
{
	t_cmd_data	*cmd;
	int			exit_code;
	int			argc;

	cmd = &node->u_data.cmd;
	argc = count_args(cmd->argv);
	ft_putendl_fd("exit", STDERR_FILENO);
	if (argc == 1)
		exit_code = program->last_exit_status;
	else if (argc == 2)
		exit_code = get_exit_code(cmd->argv[1], program->last_exit_status);
	else
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		program->last_exit_status = 1;
		return ;
	}
	cleanup_program(program);
	exit(exit_code);
}
