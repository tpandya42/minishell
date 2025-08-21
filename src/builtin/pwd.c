/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:27:29 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/19 16:44:44 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//cwd: current working directory
//Original pwd: Print the full 
//filename of the current working directory.
int	my_pwd(t_program *program, t_node *node)
{
	char	*cwd_path;

	(void) node;
	cwd_path = getcwd(NULL, 0);
	if (cwd_path)
	{
		printf("%s\n", cwd_path);
		free(cwd_path);
		return (0);
	}
	else
	{
		fprintf(stderr, BLUE "pwd: current directory not found\n" RESET);
		free(cwd_path);
		return (1);
	}
}
