/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:27:29 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/29 09:01:07 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	my_pwd(t_program *program, t_node *node)
{
	char	*cwd_path;

	(void)node;
	(void)program;
	cwd_path = getcwd(NULL, 0);
	if (cwd_path)
	{
		ft_putendl_fd(cwd_path, STDOUT_FILENO);
		free(cwd_path);
		return (0);
	}
	else
	{
		ft_putendl_fd("pwd: current directory not found", STDERR_FILENO);
		free(cwd_path);
		return (1);
	}
}
