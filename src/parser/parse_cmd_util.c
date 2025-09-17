/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_util.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:11:26 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/15 08:04:56 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_word_token(t_program *program, t_token *token)
{
	if (token->type == WORD && (ft_strchr(token->txt, '>') != NULL
			|| ft_strchr(token->txt, '<') != NULL))
	{
		ft_print_syntax_error(token->txt);
		program->last_exit_status = 2;
		return (1);
	}
	return (0);
}

int	validate_pipe_token(t_program *program, t_token *current, t_token *first)
{
	if (current->type == PIPE)
	{
		if (current == first || !current->next || current->next->type == PIPE
			|| current->next->type == REDIR_IN
			|| current->next->type == REDIR_OUT
			|| current->next->type == APPEND || current->next->type == HEREDOC)
		{
			if (!current->next)
				ft_print_syntax_error(NULL); 
			else
				ft_print_syntax_error(current->txt);
			program->last_exit_status = 2;
			return (1);
		}
	}
	return (0);
}

int	validate_redir_target(t_program *program, t_token *current)
{
	if (!current->next)
	{
		error_split_arg(current);
		program->last_exit_status = 2;
		return (1);
	}
	if (current->next->type == REDIR_IN
		|| current->next->type == REDIR_OUT
		|| current->next->type == APPEND
		|| current->next->type == HEREDOC)
	{
		error_split_arg(current);
		program->last_exit_status = 2;
		return (1);
	}
	return (0);
}

int	handle_miss_cmd(t_program *program, t_cmd_data *cmd_data,
	t_token *cmd_tokens, bool has_cmd)
{
	t_redir	*redir;

	if (!cmd_tokens && !has_cmd)
	{
		redir = cmd_data->redir;
		if (!redir)
		{
			ft_print_error("syntax error: Command expected "
				"but only redirections found");
			cmd_data->tokens = NULL;
			cmd_data->argv = NULL;
			program->last_exit_status = 2;
			return (1);
		}
	}
	return (0);
}
