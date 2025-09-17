/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 07:27:39 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/15 06:37:08 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator_char(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

// Checks multi-character operators
static enum e_toktype	multi_char_operator(char *s)
{
	if (s[0] == '>' && s[1] && s[1] == '>')
		return (APPEND);
	if (s[0] == '<' && s[1] && s[1] == '<')
		return (HEREDOC);
	return (WORD);
}

// Checks single-character operators
static enum e_toktype	single_char_operator(char c)
{
	if (c == '|')
		return (PIPE);
	if (c == '>')
		return (REDIR_OUT);
	if (c == '<')
		return (REDIR_IN);
	return (WORD);
}

// empty or null string -> treat as WORD
enum e_toktype	token_type(char *s)
{
	enum e_toktype	type;

	if (!s || !*s)
		return (WORD);
	type = multi_char_operator(s);
	if (type != WORD)
		return (type);
	type = single_char_operator(s[0]);
	if (type != WORD)
		return (type);
	if (s[0] == '\'')
		return (SINGLE_Q);
	if (s[0] == '"')
		return (DOUBLE_Q);
	return (WORD);
}

t_token	*extract_token(char *s, size_t size)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->txt = malloc(size + 1);
	if (!token->txt)
	{
		free(token);
		return (NULL);
	}
	ft_strncpy(token->txt, s, size);
	token->txt[size] = 0;
	token->type = token_type(token->txt);
	token->next = NULL;
	return (token);
}
