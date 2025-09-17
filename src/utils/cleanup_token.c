/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 08:24:19 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/10 16:48:34 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token *token)
{
	t_token	*next;

	next = NULL;
	if (!token)
		return ;
	while (token)
	{
		next = token->next;
		if (token->txt)
			free(token->txt);
		free(token);
		token = next;
	}
}

void	free_token_list(t_program *program)
{
	free_token(program->token_list);
	program->token_list = NULL;
}
