/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 11:52:09 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/29 14:31:16 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
*	@brief ends the progam and free resources needed: nodes, tokens, etc
*	@returns void because ends the program
*/
void	my_exit(t_program *program, t_node *node)
{
	(void) node;
	// fprintf(stderr, MAGENTA BOLD "MY EXIT is about to be run\n" RESET);
	DEBUG_PRINT(MAGENTA BOLD "MY EXIT is about to be run\n" RESET);
	cleanup_program(program);
	exit(program->last_exit_status);
}

