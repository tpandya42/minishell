/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 14:32:01 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/08 17:51:26 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*next_operator(t_token *token)
{
	while (token->next && token->type < PIPE)
		token = token->next;
	return (token);
}

t_token	*token_cpy(t_token *token)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->txt = ft_strdup(token->txt);
	if (!new_token->txt)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = token->type;
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **list, t_token *new_token)
{
	t_token	*tmp;

	if (!new_token)
		return ;
	if (!*list)
	{
		*list = new_token;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_token;
}

int	process_tokens_loop(t_program *program, t_token *token,
		t_cmd_data *cmd_data, t_token **cmd_tokens)
{
	t_token	*current;

	current = token;
	*cmd_tokens = NULL;
	while (current)
	{
		if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == APPEND || current->type == HEREDOC)
		{
			if (split_cmd_arg(program, &current, cmd_data) != 0)
				return (1);
			continue ;
		}
		add_token(cmd_tokens, token_cpy(current));
		current = current->next;
	}
	return (0);
}
