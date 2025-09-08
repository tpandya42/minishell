/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:35:57 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/29 13:57:05 by albetanc         ###   ########.fr       */
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
	if (token->type == SEMICOLON)
		return (1);
	if (token->type == AND || token->type == OR)
	 	return (2);
	if (token->type == PIPE)
		return (3);
	return (0);
}

t_node	*parse_command(t_token *token)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);//check malloc error and free, set message
	ft_memset(node, 0, sizeof(t_node));//new
	init_cmd_node(NULL, node);
	if (process_cmd_tokens(token, &node->u_data.cmd) != 0)
	{
		free_token(token);//new
		free_node(node);//changed from free to free_node
		DEBUG_ERROR("[ERROR] parse_command: failed to process tokens\n");//DEBUG
		return (NULL);
	}
	return (node);
}

t_node *parse_operator(t_token *op, t_node *left, t_node *right)
{
    t_node *node = malloc(sizeof(t_node));
    if (!node)//check malloc error and free, set message
    {
        free_node(left);
        free_node(right);
        return NULL;
    }
	ft_memset(node, 0, sizeof(t_node));//new
    node->type = OPERATOR;
    node->u_data.op.type = op->type; // directly use token type
    node->u_data.op.left = left;
    node->u_data.op.right = right;
    return node;
}
//precedence
//NEEDS TO BE IMPROVED FOR LEFT ASSOCIATIVE
//now if ok for right, then for pipes
// Find the lowest precedence operator for left-associativity
static t_token *find_lowest_operator(t_token *token)
{
    t_token *lowest_op = NULL;

    while (token)
    {
        int curr_prec = precedence(token);
        if (curr_prec > 0)
        {
            if (!lowest_op || curr_prec < precedence(lowest_op))
                lowest_op = token; // leftmost wins automatically
        }
        token = token->next;
    }
    return lowest_op;
}


t_node *parse(t_token *token_list)
{
    if (!token_list)
    {//debug
        // DEBUG removed//debug
        return NULL;
    }//debug

    // 1. Find operator of lowest precedence (leftmost for left-associativity)
    t_token *op_token = find_lowest_operator(token_list);

    // 2. No operator -> just a command node
    if (!op_token)
        return parse_command(token_list);

    // 3. Split tokens into left and right lists
    t_token *left_list = token_list;
    t_token *right_list = op_token->next;
    t_token *current = left_list;

    // Cut the left list at op_token
    if (current == op_token)
        left_list = NULL;
    else
    {
        while (current && current->next != op_token)
            current = current->next;
        if (current)
            current->next = NULL;
    }

    // 4. Recursively parse left and right
    t_node *left = parse(left_list);
    t_node *right = parse(right_list);

    if (!left || !right) // NEW
    {
        free_node(left);
        free_node(right);
        DEBUG_ERROR("[ERROR] parse: failed to parse left or right sub-tree\n");//debug
        return NULL;
    }
    // 5. Create operator node
    return parse_operator(op_token, left, right);
}
