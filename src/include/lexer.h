/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 08:36:45 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/15 07:23:45 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

t_token			*lex(char *s, t_program *program);
enum e_toktype	token_type(char *s);
t_token			*join_tokens(t_token *a, t_token *b);

// --- Quotes --- //
t_token			*lex_quoted(t_program *program, char *s, char quote);
t_token			*lex_unquoted(t_program *program, char *s);
t_token			*extract_token(char *s, size_t size);
t_token			*handle_adj_token(t_program *program,
					t_token *token, char *rest);

// --- Helpers --- //
int				ft_isspace(char c);
char			*consume_whitespace(char *head_token);
int				is_operator_char(char c);

#endif
