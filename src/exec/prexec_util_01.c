/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prexec_util_01.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:07:06 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/08 15:07:07 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_count_words(const char *str)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	if (!str)
		return (0);
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
		{
			if (!in_word)
			{
				count++;
				in_word = 1;
			}
		}
		else
			in_word = 0;
		str++;
	}
	return (count);
}

void	free_partial_arr(char **arr, int allocated_i)
{
	if (!arr)
		return ;
	while (allocated_i > 0)
	{
		free(arr[allocated_i - 1]);
		allocated_i--;
	}
	free(arr);
}

int	count_tokens(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		count++;
		token = token->next;
	}
	return (count);
}

int	is_operator_token(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == PIPE || token->type == AND || token->type == OR
		|| token->type == SEMICOLON)
		return (1);
	return (0);
}

t_token	*skip_redirs(t_token *tok)
{
	if (!tok)
		return (NULL);
	if (tok->type == REDIR_IN || tok->type == REDIR_OUT
		|| tok->type == APPEND || tok->type == HEREDOC)
	{
		if (tok->next)
			return (tok->next->next);
		return (tok->next);
	}
	return (tok);
}
