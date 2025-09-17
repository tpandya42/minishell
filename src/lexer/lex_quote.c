/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_quote.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 07:28:27 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/16 07:56:06 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*join_tokens(t_token *a, t_token *b)
{
	size_t	len_a;
	size_t	len_b;
	char	*joined;

	if (!a)
		return (b);
	if (!b)
		return (a);
	len_a = ft_strlen(a->txt);
	len_b = ft_strlen(b->txt);
	joined = malloc(len_a + len_b + 1);
	if (!joined) 
	{
		free_token(b);
		return (a);
	}
	ft_strcpy(joined, a->txt);
	ft_strcat(joined, b->txt);
	free(a->txt);
	a->txt = joined;
	a->next = b->next;
	free(b->txt);
	free(b);
	return (a);
}

//find the end of the quoted str
// skip escape in double quotes
static char	*find_quote_end(char *start, char quote)
{
	char	*end;

	end = start;
	while (*end && *end != quote)
	{
		if (quote == '"' && *end == '\\' && (*(end + 1) == '"'
				|| *(end + 1) == '$' || *(end + 1) == '\\'))
			end += 2;
		else
			end++;
	}
	return (end);
}

//processes adjacent token without spaces and joins it to the current token
t_token	*handle_adj_token(t_program *program, t_token *token, char *rest)
{
	t_token	*next;

	if (*rest && !ft_isspace(*rest) && !is_operator_char(*rest))
	{
		if (*rest == '\'' || *rest == '"')
			next = lex_quoted(program, rest + 1, *rest);
		else
			next = lex_unquoted(program, rest);
		if (!next)
		{
			free_token(token);
			return (NULL);
		}
		token = join_tokens(token, next);
	}
	else
		token->next = lex(consume_whitespace(rest), program);
	return (token);
}

t_token	*lex_quoted(t_program *program, char *s, char quote)
{
	t_token	*token;
	char	*end;
	char	*rest;

	end = find_quote_end(s, quote);
	if (!*end)
	{
		ft_print_error("Syntax error: unclosed quote");
		return (NULL);
	}
	token = extract_token(s, end - s);
	if (!token)
		return (NULL);
	add_token(&program->token_list, token);
	if (quote == '\'')
		token->type = SINGLE_Q;
	else
		token->type = DOUBLE_Q;
	rest = end + 1;
	return (handle_adj_token(program, token, rest));
}
