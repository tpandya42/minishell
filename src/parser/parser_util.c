/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 08:22:30 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/23 12:04:24 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	init_cmd_node(t_token *token, t_node *node)
{
	node->type = COMMAND;
	node->u_data.cmd.tokens = token;
	node->u_data.cmd.redir = NULL;
	node->u_data.cmd.argv = NULL;
	node->u_data.cmd.cmd_type = EXECUTABLE;
	node->u_data.cmd.fd_in = STDIN_FILENO;
	node->u_data.cmd.fd_out = STDOUT_FILENO;
}

t_token	*token_cpy(t_token *token)
{
	t_token	*new_token;

	new_token = malloc (sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->txt = ft_strdup(token->txt);
	if (!new_token->txt)
	{
		free (new_token);//check if smt else
		return (NULL);
	}
	new_token->type = token->type;
	new_token->next = NULL;//check where to free new_token
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
		tmp = tmp ->next;
	tmp->next = new_token;
}

void	error_split_arg(t_token *current)
{
	if (current->next)
		fprintf(stderr, BOLD RED
			"syntax error near unexpected token '%s'\n" RESET,
			current->next->txt);
	else
		fprintf(stderr, BOLD RED
			"syntax error near unexpected token 'newline'\n" RESET);
}

int	split_cmd_arg(t_token **current, t_cmd_data *cmd_data)
{
	if (!(*current)->next || (*current)->next->type != WORD)
	{
		error_split_arg(*current);
		return (1);
	}
	add_redir(&cmd_data->redir,
		create_redir_node((*current)->next->txt, map_type((*current)->type)));
	*current = (*current)->next->next;
	return (0);
}


int	process_cmd_tokens(t_token *token, t_cmd_data *cmd_data)
{
	t_token	*current;
	t_token	*cmd_tokens;

	cmd_tokens = NULL;
	current = token;
	while (current)
	{
		if (current ->type >= REDIR_IN && current->type <= HEREDOC)
		{
			if (split_cmd_arg(&current, cmd_data) != 0)
			{
				free_token(cmd_tokens);
				return (1);
			}
		}
		else
		{
			add_token(&cmd_tokens, token_cpy(current));
			current = current->next;
		}
	}
	cmd_data->tokens = cmd_tokens;
	return (0);
}
