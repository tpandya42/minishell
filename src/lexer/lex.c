/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 08:57:29 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/28 15:50:13 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int is_operator_char(char c)
{
	return (c == '|' || c == '&' || c == '>' || c == '<' || c == ';' || c == '(' || c == ')');
}

enum e_toktype token_type(char *s)
{
	if (!s || !*s)
		return WORD; // empty or null string -> treat as WORD

	// Multi-character operators (check length first)
	if (s[0] == '|' && s[1] && s[1] == '|')
		return OR;
	if (s[0] == '&' && s[1] && s[1] == '&')
		return AND;
	if (s[0] == '>' && s[1] && s[1] == '>')
		return APPEND;
	if (s[0] == '<' && s[1] && s[1] == '<')
		return HEREDOC;

	// Single-character operators
	if (s[0] == '|')
		return PIPE;
	if (s[0] == ';')
		return SEMICOLON;
	if (s[0] == '>')
		return REDIR_OUT;
	if (s[0] == '<')
		return REDIR_IN;
	if (s[0] == '(')
		return OPEN;
	if (s[0] == ')')
		return CLOSE;

	// Quotes
	if (s[0] == '\'')
		return SINGLE_Q;
	if (s[0] == '"')
		return DOUBLE_Q;

	// Default fallback
	return WORD;
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
	strncpy(token->txt, s, size);
	token->txt[size] = 0;
	token->type = token_type(token->txt);
	return (token);
}

char	*consume_whitespace(char *head_token)
{
	while (*head_token && (*head_token == ' ' || *head_token == '\t'))
		head_token++;
	return (head_token);
}


// t_token	*lex(char *s, char delim)//make it shorter
// {
// 	char	*head_token;
// 	char	*current;
// 	t_token	*token;

// 	head_token = s;
// 	current = head_token;
// 	while (*current && *current!= delim)
// 		current++;
// 	if (!*current&& delim != ' ')
// 	{
// 		perror("unclosed quote");
// 		return (NULL);
// 	}
// 	token = extract_token(head_token, current - head_token);
// 	if (!token)
// 		return (NULL);
// 	token->delim = delim;
// 	current = consume_whitespace(current);
// 	if (*current)
// 	{
// 		if (*current== '\'' || *current== '"')
// 			token->next = lex(current + 1, *current);
// 		else
// 			token->next = lex(current, ' ');
// 		if (token->next == NULL && *current)
// 		{
// 			free(token->txt);
// 			free(token);
// 			return (NULL); 
// 		}
// 	}
// 	else
// 		token->next = NULL;
// 		return (token);
// }

static int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
			|| c == '\v' || c == '\f' || c == '\r');
}

static t_token *join_tokens(t_token *a, t_token *b)
{
	if (!a) return b;
	if (!b) return a;

	size_t len_a = strlen(a->txt);
	size_t len_b = strlen(b->txt);

	char *joined = malloc(len_a + len_b + 1);
	if (!joined) 
	{
		// malloc_error();//check
		free_token(b);//NEW
		return (a); // leak risk if malloc fails
	}
	strcpy(joined, a->txt);
	strcat(joined, b->txt);

	free(a->txt);
	a->txt = joined;

    // preserve the rest of b's chain
    t_token *b_next = b->next;
    free(b->txt);
    free(b);

    a->next = b_next;
	return a;
}


t_token *lex_quoted(char *s, char quote)
{
    t_token *token;
    t_token *next;
    char *start = s;
    char *end = s;
    char *rest;

    while (*end && *end != quote)
    {
        if (quote == '"' && *end == '\\' && (*(end + 1) == '"' || *(end + 1) == '$' || *(end + 1) == '\\'))
            end += 2; // skip escape in double quotes
        else
            end++;
    }

    if (!*end)
    {
        perror("Unclosed quote");
        return NULL;
    }

    token = extract_token(start, end - start);
    if (!token)
        return NULL;

    token->type = (quote == '\'') ? SINGLE_Q : DOUBLE_Q;

    rest = end + 1;

    // Check for adjacent token without whitespace
    if (*rest && !ft_isspace(*rest) && !is_operator_char(*rest))
    {
        if (*rest == '\'' || *rest == '"')
            next = lex_quoted(rest + 1, *rest);
        else
            next = lex_unquoted(rest);

		if (!next)
		{
			free_token(token);//new
			return (NULL);
		}

        token = join_tokens(token, next);
    }
    else
        token->next = lex(consume_whitespace(rest), ' ');

    return token;
}

t_token *lex_unquoted(char *s)
{
    char *start = s;
    char *end = s;
    char *rest;
    t_token *token;
    t_token *next;

    while (*end && !ft_isspace(*end) && !is_operator_char(*end) && *end != '\'' && *end != '"')
    {
        if (*end == '\\' && *(end + 1)) // handle escape
            end += 2;
        else
            end++;
    }

    token = extract_token(start, end - start);
    if (!token)
        return NULL;

    token->type = WORD;

    // Check for redirection characters within the word
    if (strchr(token->txt, '>') != NULL || strchr(token->txt, '<') != NULL) {
        // If the word contains redirection chars, check if they're at positions where
        // they'd form valid redirection syntax (beginning of word or after space)
        for (size_t i = 0; token->txt[i]; i++) {
            if ((token->txt[i] == '>' || token->txt[i] == '<') && i > 0 && token->txt[i-1] != ' ') {
                fprintf(stderr, BOLD RED "Syntax error: Invalid redirection syntax in '%s'\n" RESET, token->txt);
                free_token(token);
                return NULL;
            }
        }
    }

    rest = end;

    // Check for adjacent token without whitespace
    if (*rest && !ft_isspace(*rest) && !is_operator_char(*rest))
    {
        if (*rest == '\'' || *rest == '"')
            next = lex_quoted(rest + 1, *rest);
        else
            next = lex_unquoted(rest);

        if (!next)
		{
			free_token(token);//new
			return (NULL);
		}
        token = join_tokens(token, next);
    }
    else
        token->next = lex(consume_whitespace(rest), ' ');

    return token;
}

t_token *lex(char *s, char delim)
{
	if (!s || !*s) return NULL;
	s = consume_whitespace(s);
	if (!*s) return NULL;

	if (*s == '\'' || *s == '"')           // Quoted string
		return lex_quoted(s + 1, *s);
	else if (is_operator_char(*s))         // Operator
	{
		size_t op_len = 1;

		// Detect multi-char operators
		if ((*s == '|' && *(s+1) == '|') || 
				(*s == '&' && *(s+1) == '&') ||
				(*s == '>' && *(s+1) == '>') ||
				(*s == '<' && *(s+1) == '<'))
			op_len = 2;

		t_token *token = extract_token(s, op_len);
		if (!token) return NULL;

		token->type = token_type(token->txt);
		token->next = lex(consume_whitespace(s + op_len), ' ');
		return token;
	}
	else                                   // Normal word
		return lex_unquoted(s);
}

int print_tokens(t_token *token)
{
	int len = 0;
	while (token)
	{
		len += printf("%s ", token->txt);
		token = token->next;
	}
	return len;
}
