/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 08:17:46 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/15 17:03:41 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEANUP_H
# define CLEANUP_H

# include "minishell.h"

// --- Program --- //
void	malloc_error(t_program *program);
void	cleanup_program(t_program *program);
void	free_ast_tokens(t_program *program);
void	free_envp(char **envp);

// --- Tokens --- //
void	free_token(t_token *token);
void	free_token_list(t_program *program);

// --- Fd --- //
int		cleanup_fd(t_node *node, t_nodetype type);
void	cleanup_fds(t_cmd_data *cmd);
int		cleanup_cmd_node(t_node *node);
int		cleanup_operator_fd(t_node *node);

// --- Node ---//
void	free_node(t_node *node);
void	free_redirs(t_redir *redir);
void	free_cmd_arg(t_node *node);

#endif
