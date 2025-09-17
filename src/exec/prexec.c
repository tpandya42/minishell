/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prexec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 08:41:11 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/16 07:38:01 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_cmd_tokens(t_token *tok)
{
	int	count;

	count = 0;
	while (tok && !is_operator_token(tok))
	{
		if (is_redir_token(tok))
			tok = skip_redirs(tok);
		else
		{
			count++;
			tok = tok->next;
		}
	}
	return (count);
}

static int	cpy_token_str(char **argv, t_token *token)
{
	t_token	*cur;
	int		i;

	cur = token;
	i = 0;
	while (cur && !is_operator_token(cur))
	{
		if (is_redir_token(cur))
			cur = skip_redirs(cur);
		else
		{
			if (copy_word(argv, cur, i) != 0)
				return (1);
			cur = cur->next;
			i++;
		}
	}
	argv[i] = NULL;
	return (0);
}

char	**token_to_argv(t_token *token)
{
	char	**argv;
	int		nb_token;

	nb_token = count_cmd_tokens(token);
	argv = malloc(sizeof(char *) * (nb_token + 1));
	if (!argv)
		return (NULL);
	if (cpy_token_str(argv, token) != 0)
	{
		free(argv);
		return (NULL);
	}
	return (argv);
}

void	setup_cmd_arg(t_program *program, t_node *node)
{
	node->u_data.cmd.argv = token_to_argv(node->u_data.cmd.tokens);
	if (!node->u_data.cmd.argv)
	{
		cleanup_program(program);
		exit(EXIT_FAILURE);
	}
}

void	pre_execution(t_program *program, t_node *node)
{
	if (!node)
		return ;
	if (node->type == OPERATOR)
	{
		pre_execution(program, node->u_data.op.left);
		pre_execution(program, node->u_data.op.right);
	}
	else if (node->type == COMMAND)
	{
		setup_cmd_arg(program, node);
		apply_redir(program, node);
		node->u_data.cmd.env = program->envp;
		if (node->u_data.cmd.argv[0] && is_builtin(node->u_data.cmd.argv[0]))
			node->u_data.cmd.cmd_type = BUILTIN;
		else
			node->u_data.cmd.cmd_type = EXECUTABLE;
	}
}
