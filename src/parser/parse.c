/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:35:57 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/11 08:49:29 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	precedence(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == PIPE)
		return (3);
	return (0);
}

//precedence
//now if ok for right, then for pipes
// Find the lowest precedence operator for left-associativity
static t_token	*find_lowest_operator(t_token *token)
{
	t_token	*lowest_op;
	int		current_prec;

	lowest_op = NULL;
	while (token)
	{
		current_prec = precedence(token);
		if (current_prec > 0)
		{
			if (!lowest_op || current_prec < precedence(lowest_op))
				lowest_op = token;
		}
		token = token->next;
	}
	return (lowest_op);
}

// Cut the left list at op_token
//oken lit always begn in left
static t_token	*cut_left_list(t_token *token_list, t_token *op_token)
{
	t_token	*current;

	current = token_list;
	if (current == op_token)
		return (NULL);
	while (current && current->next != op_token)
		current = current->next;
	if (current)
		current->next = NULL;
	return (token_list);
}

// Recursively parse left and right
//creae operator node
static t_node	*parse_sides(t_program *program, t_token *op_token,
	t_token *left_list, t_token *right_list)
{
	t_node	*left;
	t_node	*right;

	left = parse(program, left_list);
	right = parse(program, right_list);
	if (!left || !right)
	{
		free_node(left);
		free_node(right);
		return (NULL);
	}
	return (parse_operator(op_token, left, right));
}

// Find operator of lowest precedence (leftmost for left-associativity)
// No operator -> just a command node
// Split tokens into left and right lists
t_node	*parse(t_program *program, t_token *token_list)
{
	t_token	*op_token;
	t_token	*left_list;
	t_token	*right_list;

	if (!token_list)
		return (NULL);
	op_token = find_lowest_operator(token_list);
	if (!op_token)
		return (parse_command(program, token_list));
	right_list = op_token->next;
	left_list = cut_left_list(token_list, op_token);
	return (parse_sides(program, op_token, left_list, right_list));
}
