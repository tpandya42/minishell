/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 14:29:17 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/29 14:30:48 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_no_newline_flag(char **argv, int *i)
{
	if (argv[*i] && ft_strncmp(argv[*i], "-n", 3) == 0)
	{
		(*i)++;
		return (0);
	}
	return (1);
}

static void	print_args(char **argv, int i, int fd_out)
{
	while (argv[i])
	{
		write(fd_out, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(fd_out, " ", 1);
		i++;
	}
}

int	my_echo(t_program *program, t_node *node)
{
	int	i;
	int	new_line;
	int	fd_out;

	(void)program;
	fd_out = STDOUT_FILENO;
	i = 1;
	new_line = has_no_newline_flag(node->u_data.cmd.argv, &i);
	if (!node->u_data.cmd.argv[i])
	{
		if (new_line)
			write(fd_out, "\n", 1);
		return (0);
	}
	print_args(node->u_data.cmd.argv, i, fd_out);
	if (new_line)
		write(fd_out, "\n", 1);
	fsync(fd_out);
	return (0);
}
