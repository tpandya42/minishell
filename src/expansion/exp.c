/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 12:34:08 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/10 12:34:09 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static char	*join_free(char *result, char *val)
{
	char	*tmp;

	tmp = ft_strjoin(result, val);
	free(result);
	free(val);
	return (tmp);
}

char	*expand_token_text(char *txt, char **envp_cpy, int last_exit)
{
	char	*result;
	char	*pos;
	char	*val;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	pos = txt;
	while (*pos)
	{
		if (*pos == '$' && *(pos + 1))
		{
			if (*(pos + 1) == '?' || *(pos + 1) == '$')
			{
				val = status_exp(*(pos + 1), last_exit);
				pos += 2;
			}
			else
				val = expand_token_helper_01(&pos, envp_cpy);
			result = join_free(result, val);
		}
		else
			result = expand_token_helper(result, pos++);
	}
	return (result);
}

void	expand(t_token *token, char **envp_cpy, int last_exit)
{
	t_token	*current;
	char	*expanded;

	current = token;
	while (current)
	{
		if (current->type == WORD || current->type == DOUBLE_Q)
		{
			expanded = expand_token_text(current->txt, envp_cpy, last_exit);
			if (!expanded)
				return ;
			free(current->txt);
			current->txt = expanded;
		}
		current = current->next;
	}
}
