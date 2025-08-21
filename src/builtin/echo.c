/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 14:29:17 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/01 08:21:42 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//echo - display a line of text
//-e enable interpretation of backslash escapes
// - Repeat multiple times with different arguments

/*
*   1. new_line with 1 means there is '\n' and 0 means no.
*   2. Check if the first arg is -n
*   3. Write each arg with spaces if is not the last one
*   4. Includes '\n' if there is no -n in the args
*/
// int	my_echo(t_node *node)
int	my_echo(t_program *program, t_node *node)
{
	int		i;
	int		new_line;

	(void) program;
	fprintf(stderr, MAGENTA BOLD "MY ECHO is about to be run\n" RESET);
	i = 1;//from 1 are the cmd's arg
	new_line = 1;
	if (node->u_data.cmd.argv[i] && ft_strncmp(node->u_data.cmd.argv[i], "-n", 3) == 0)//3 including '\0'
	{
		new_line = 0;
		i++;
	}
	while (node->u_data.cmd.argv[i])//loop to str per str
	{
		if (safe_write(node->u_data.cmd.fd_out, node->u_data.cmd.argv[i],
				ft_strlen(node->u_data.cmd.argv[i])) == -1)//implement correctly
			return (1);
		if (node->u_data.cmd.argv[i + 1])
		{
			if (safe_write(node->u_data.cmd.fd_out, " ", 1) == -1)
				return (1);
		}
		i++;
	}
	if (new_line)//handles -n
	{
		if (safe_write(node->u_data.cmd.fd_out, "\n", 1) == -1)
			return (1);
	}
	return (0);
}
