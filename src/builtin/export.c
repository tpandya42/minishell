/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rococo <rococo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 11:12:31 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/15 15:20:08 by rococo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
*	@brief Prints all currently exported environment variables
*	in the declare -x format
*	@returns 0 on success and 1 on error
*/
// int	my_export(char **envp)
int	my_export(t_program *program, t_node *node)
{
	int		i;
	char	**args;
	int		nb_args;
	char	*equals_sign;

	fprintf(stderr, MAGENTA BOLD "MY EXPORT is about to be run\n" RESET);
	args = node->u_data.cmd.argv;
	nb_args = 0;
	while (args && args[nb_args])
		nb_args++;
	if (nb_args == 1)
	{
		if (!program->envp_cpy)
		{
			fprintf(stderr, BLUE "export: no environment variable found\n" RESET);
			return (1);
		}
		i = 0;
		while (program->envp_cpy[i])
		{
			equals_sign = ft_strchr(program->envp_cpy[i], '=');;
			if (equals_sign)
			{
				*equals_sign = '\0';
				printf("declare -x %.*s=\"%s\"\n", (int)(equals_sign - program->envp_cpy[i]), program->envp_cpy[i], equals_sign + 1);
				*equals_sign = '=';
			}
			else
				printf("declare -x %s\n", program->envp_cpy[i]);
			i++;
		}
	}
	return (0);
}

