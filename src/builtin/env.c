/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:55:37 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/29 14:31:03 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	my_env(t_program *program, t_node *node)
{
	int	i;

	(void)node;
	if (!program->envp_cpy)
	{
		return (1);
	}
	i = 0;
	while (program->envp_cpy[i])
	{
		safe_write(STDOUT_FILENO, program->envp_cpy[i],
			ft_strlen(program->envp_cpy[i]));
		safe_write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	return (0);
}
