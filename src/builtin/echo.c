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


// ---->> udated my echo function to also read from STDIN so that it can also support heredoc
int my_echo(t_program *program, t_node *node)
{
	int i;
	int new_line;
	int fd_out;

	(void) program;
	
	// Make sure we're using the correct file descriptor
	fd_out = node->u_data.cmd.fd_out;
	
	i = 1; // starts after command name
	new_line = 1;

	// Handle -n option
	if (node->u_data.cmd.argv[i] && ft_strncmp(node->u_data.cmd.argv[i], "-n", 3) == 0)
	{
		new_line = 0;
		i++;
	}

	if (!node->u_data.cmd.argv[i]) // No args: just print newline (unless -n)
	{
		if (new_line)
			write(fd_out, "\n", 1);
		return 0;
	}

	// Print args
	while (node->u_data.cmd.argv[i])
	{
		write(fd_out, node->u_data.cmd.argv[i], ft_strlen(node->u_data.cmd.argv[i]));
		if (node->u_data.cmd.argv[i + 1])
			write(fd_out, " ", 1);
		i++;
	}
	if (new_line)
		write(fd_out, "\n", 1);
	
	// Make sure the data is written to disk
	fsync(fd_out);
	
	return 0;
}
