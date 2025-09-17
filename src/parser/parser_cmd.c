/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 14:31:48 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/16 07:36:48 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_cmd_node(t_token *token, t_node *node)
{
	ft_memset(node, 0, sizeof(t_node));
	node->type = COMMAND;
	node->u_data.cmd.tokens = token;
	node->u_data.cmd.redir = NULL;
	node->u_data.cmd.argv = NULL;
	node->u_data.cmd.cmd_type = EXECUTABLE;
	node->u_data.cmd.fd_in = STDIN_FILENO;
	node->u_data.cmd.fd_out = STDOUT_FILENO;
	node->u_data.cmd.pipefd[0] = -1;
	node->u_data.cmd.pipefd[1] = -1;
}

static int	validate_redirections(t_program *program, t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == APPEND || current->type == HEREDOC)
		{
			if (validate_redir_target(program, current))
				return (1);
		}
		current = current->next;
	}
	return (0);
}

static int	validate_token_syntax(t_program *program, t_token *token,
		bool *has_command)
{
	t_token	*current;
	t_token	*first;

	current = token;
	first = token;
	*has_command = false;
	while (current)
	{
		if (current->type == WORD || current->type == SINGLE_Q
			|| current->type == DOUBLE_Q)
		{
			if (validate_word_token(program, current))
				return (1);
			*has_command = true;
		}
		if (validate_pipe_token(program, current, first))
			return (1);
		current = current->next;
	}
	if (validate_redirections(program, token))
		return (1);
	return (0);
}

static int	free_cmd_tokens_and_fail(t_cmd_data *cmd_data, t_token *cmd_tokens)
{
	free_token(cmd_tokens);
	cmd_data->tokens = NULL;
	cmd_data->argv = NULL;
	return (1);
}

int	process_cmd_tokens(t_program *program, t_token *token, t_cmd_data *cmd_data)
{
	t_token	*cmd_tokens;
	bool	has_cmd;

	if (validate_token_syntax(program, token, &has_cmd) != 0)
	{
		cmd_data->tokens = NULL;
		cmd_data->argv = NULL;
		return (1);
	}
	if (process_tokens_loop(program, token, cmd_data, &cmd_tokens) != 0)
		return (free_cmd_tokens_and_fail(cmd_data, cmd_tokens));
	if (handle_miss_cmd(program, cmd_data, cmd_tokens, has_cmd))
		return (free_cmd_tokens_and_fail(cmd_data, cmd_tokens));
	cmd_data->tokens = cmd_tokens;
	cmd_data->argv = NULL;
	return (0);
}
