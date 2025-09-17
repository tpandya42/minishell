/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 07:34:15 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/16 07:35:15 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_pipe_edges(t_token *token_list, t_program *program)
{
	t_token	*last;

	if (token_list->type == PIPE)
	{
		ft_print_syntax_error("|");
		program->last_exit_status = 2;
		free_token_list(program);
		return (0);
	}
	last = token_list;
	while (last->next)
		last = last->next;
	if (last->type == PIPE)
	{
		ft_print_syntax_error("|");
		program->last_exit_status = 2;
		free_token_list(program);
		return (0);
	}
	return (1);
}
