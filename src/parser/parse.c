/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:35:57 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/23 12:03:02 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// --- FORWARD DECLARATIONS --- //
t_node			*parse_command(t_token *token);
static t_token	*next_operator(t_token *token);
static int		precedence(t_token *token);

static t_token	*next_operator(t_token *token)
{
	while (token->next && token->type < PIPE)
		token = token->next;
	return (token);
}

static int	precedence(t_token *token)
{
	if(!token)
		return (0);
	// if (token->type == SEMICOLON)
	// 	return (1);
	// if (token->type == AND || token->type == OR)
	// 	return (2);
	if (token->type == PIPE)
		return (3);
	return (0);
}

t_node	*parse_command(t_token *token)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);//check malloc error and free
	init_cmd_node(NULL, node);
	if (process_cmd_tokens(token, &node->u_data.cmd) != 0)
	{
		// free_token(token);//check
		free(node);//check
		return (NULL);
	}
	return (node);
}

t_node	*parse_operator(t_token *op, t_node *left, t_node *right)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
	{
		free_node(left);
		free_node(right);
	}
	node->type = OPERATOR;
	node->u_data.op.type = token_type(op->txt);
	node->u_data.op.left = left;
	node->u_data.op.right = right;
	return (node);
}

//precedence
//NEEDS TO BE IMPROVED FOR LEFT ASSOCIATIVE
//now if ok for right, then for pipes
static t_token	*find_lowest_operator(t_token *token)
{
	t_token	*lowest_op;
	t_token	*current;

	lowest_op = NULL;
	current = token;

	while (current)
	{
		if (precedence(current) > 0 && (!lowest_op
				|| precedence(current) < precedence(lowest_op)))
			lowest_op = current;
		current = current->next;
	}
	return (lowest_op);
}

t_node	*parse(t_token *token_list)
{
	t_token	*op_token;
	t_token	*current;
	t_node	*left;
	t_node	*right;
	t_token	*right_list;

	if (!token_list)
		return (NULL);
	op_token = find_lowest_operator(token_list);
	if (!op_token)
		return (parse_command(token_list));
	right_list = op_token->next;
	current = token_list;
	while (current && current->next != op_token)
		current = current->next;
	if (current)
		current->next = NULL;
	left = parse(token_list);
	right = parse(right_list);
	return (parse_operator(op_token, left, right));
}
