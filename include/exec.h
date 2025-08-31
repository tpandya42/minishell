/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 11:02:07 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/22 11:30:42 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"
# include <stdbool.h> //bool

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

// int		process_node(t_program *program);

// --- ABSOLUTE PATH --- //

char	*find_path(char *argv);

// --- EXECUTION --- //
//Parent cmd node
// int		fork_handle(pid_t *token_headid, t_node *node, int i_cmd, int nb_cmd);
// int		setup_redir(int fd_in, int fd_out, t_fd_dup *dup);
int		execution(t_program *program, t_node *node, bool is_pipe_child);
int		handle_cmd_exec(t_program *program, t_node *node, bool is_pipe_child);
int		exec_cmd_nopipe(t_program *token_headromgra, t_node *node);

//child
void	child_process(t_program *program, t_node *node);

//pipelines
void	exec_cmd_inpipe(t_node *node);
int		execute_pipeline(t_program *program, t_node *node);
int		wait_children(pid_t left_pid, pid_t right_pid, int *right_status);

/**
Left-associative
EXAMPLE: cat file.txt | grep "foo" | sort >> sorted.txt

          PIPE (root)
         /          \
   PIPE              COMMAND (sort)
  /     \            argv = ["sort", NULL]
COMMAND  COMMAND     infile = NULL
  (cat)   (grep)    outfile = "sorted.txt"
argv =  ["cat", NULL] append = 1  (because of >>)
infile = "file.txt"  
outfile = NULL
append = 0

argv = ["grep", "foo", NULL]
infile = NULL
outfile = NULL
append = 0
*/

#endif