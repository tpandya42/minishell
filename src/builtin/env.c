/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:55:37 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/11 13:59:45 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//return 0 for success
// int	my_env(char **envp)
int	my_env(t_program *program, t_node *node)
{
	int	i;

	(void)node;
	fprintf(stderr, MAGENTA BOLD "MY ENV is about to be run\n" RESET);
	if (!program->envp_cpy)
	{
		fprintf(stderr, BLUE "env: no environment variables found\n");
		return (1);
	}
	i = 0;
	while (program->envp_cpy[i])
	{
		printf("%s\n", program->envp_cpy[i]);
		i++;
	}
	return (0);
}
