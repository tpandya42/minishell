/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 11:02:07 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/26 15:17:49 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"
# include <stdbool.h>

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

// int		process_node(t_program *program);
typedef int	t_pipe[2];

// --- ABSOLUTE PATH --- //
char		**get_path(void);
char		*create_path(char *dir, char *argv);
char		*check_path(char **dir, char *file_path, int i);
char		*free_prev_dir(char **dir, int i);
int			is_executable(char *path);
char		*find_in_path_dirs(char **dir, char *argv);
char		*find_path(char *argv);

// pipe -> helpers

bool		has_redir_out(t_redir *redir);
bool		has_redir_in(t_redir *redir);
int			count_pipeline_nodes(t_node *node);
void		collect_pipeline_nodes(t_node *node, t_node **arr, int *idx);
int			open_pipes(int (*pipes)[2], int n);
void		close_all_pipes(int (*pipes)[2], int n);
void		init_cmd_pipefds(t_cmd_data *cmd, int i, int n_cmds,
				int (*pipes)[2]);
void		close_unused_pipes(t_cmd_data *cmd, int n_cmds, int (*pipes)[2]);
void		perform_exec(t_node *node, t_program *prog, t_cmd_data *cmd);
void		child_process_pipe(t_node *node, t_pipe_ctx *ctx, t_program *prog);
int			wait_children_pipe(pid_t *pids, int n_cmds);
t_node		**alloc_cmds(t_node *root, int n_cmds);
t_pipe		*alloc_pipes_and_open(int n_pipes);
void		free_resources(t_node **cmds, int (*pipes)[2], pid_t *pids);
int			execute_pipefile(t_program *program, t_node *node);
int			is_redir_token(t_token *tok);

// --- EXECUTION --- //
int			is_operator_token(t_token *token);
int			ft_count_words(const char *str);
t_token		*skip_redirs(t_token *tok);
int			copy_word(char **argv, t_token *tok, int i);
void		free_partial_arr(char **arr, int allocated_i);
int			count_tokens(t_token *token);
int			execution(t_program *program, t_node *node, bool is_pipe_child);
int			handle_cmd_exec(t_program *program, t_node *node,
				bool is_pipe_child);
int			exec_cmd_nopipe(t_program *token_promgra, t_node *node);

// child
void		child_process(t_program *program, t_node *node);

// pipelines
void		exec_cmd_inchild(t_node *node);
int			execute_pipeline(t_program *program, t_node *node);
int			wait_children(pid_t left_pid, pid_t right_pid, int *right_status);
void		set_final_fds(t_cmd_data *cmd);

// Helper functions

int			handle_operator(t_program *program, t_node *node,
				bool is_pipe_child);
void		set_pipe_input(t_cmd_data *cmd);
void		set_input_redir(t_cmd_data *cmd);
void		set_pipe_output(t_cmd_data *cmd);
void		set_output_redir(t_cmd_data *cmd);
int			handle_builtin_no_pipe(t_program *program, t_node *node,
				t_cmd_data *cmd);
void		child_exec_flow(t_program *program, t_node *node, t_cmd_data *cmd);
int			external_no_pipe(t_program *program, t_node *node, t_cmd_data *cmd);

#endif
