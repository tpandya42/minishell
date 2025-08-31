/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:47:36 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/22 18:34:48 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// -----------------------------------------//
//              COMMON MACROS               //
// -----------------------------------------//

//include them here if needed 
//#define MAX_ARGS 256 //checkif needed
#define BUFFER_SIZE 1024//to read from command line, from fd or from here_doc
                          //check if the buffer size is ok or if we want to chang it

// -----------------------------------------//
//        STANDARD HEADERS (COMMON)         //
// -----------------------------------------//
//STDIO.H FOR: printf, perror
//STDLIB.H FOR: malloc, free, exit, getenv
//UNISTD.H FOR: read, write, access and others
//may be here also later: sys/wait, sys/types: size_t, pid_t, strings.h
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

// -----------------------------------------//
//             MODULE HEADERS               //
// -----------------------------------------//

# include "../libft/libft.h"
# include "structs.h"
# include "colors.h"
# include "lexer.h"
# include "parser.h"
# include "prexec.h"
# include "exec.h"
# include "redir.h"
# include "builtin.h"
# include "expansion.h"
# include "export.h"
# include "unset.h"
// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

// --- PROGRAM INITIALIZATION --- //
void	init_program(t_program *program, char **envp);

// --- ERRORS_CLEAN-UP --- //
int		cleanup_fd(t_node *node, t_nodetype type);
int		cleanup_cmd_node(t_node *node);
void	cleanup_program(t_program *program);
void	free_node(t_node *node);
void	free_token(t_token *token);
void	free_ast_tokens(t_program *program);

#endif
