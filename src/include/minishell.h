/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:47:36 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/16 07:35:03 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// -----------------------------------------//
//              COMMON MACROS               //
// -----------------------------------------//

// to read from command line, from fd or from here_doc
# define BUFFER_SIZE 1024

// -----------------------------------------//
//        STANDARD HEADERS (COMMON)         //
// -----------------------------------------//

# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

// -----------------------------------------//
//             MODULE HEADERS               //
// -----------------------------------------//

# include "../libft/libft.h"
# include "structs.h"
# include "builtin.h"
# include "cleanup.h"
# include "colors.h"
# include "exec.h"
# include "expansion.h"
# include "export.h"
# include "heredoc.h"
# include "lexer.h"
# include "parser.h"
# include "prexec.h"
# include "redir.h"
# include "signall.h"
# include "unset.h"

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

// --- PROGRAM INITIALIZATION --- //
void	init_program(t_program *program, char **envp);

// --- SYNTAX --- //
int		validate_pipe_edges(t_token *token_list, t_program *program);

// --- PRINTING --- //
void	ft_print_error(const char *msg);
void	ft_print_syntax_error(const char *token);
void	print_warn_ctrl_d(int line_num, const char *delimiter);

#endif