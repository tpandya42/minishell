/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:31:14 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/26 08:29:59 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIR_H
# define REDIR_H

# include "minishell.h"

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

t_redir_type	map_type(t_toktype token_type);
t_redir			*create_redir_node(char *target, enum e_redir_type type, bool quoted);
void			add_redir(t_redir **list, t_redir *new_redir);
int				open_redir_filename(t_redir *redir);
int				process_redir(t_cmd_data *cmd, t_program *program);
void			restore_std(t_program *program);
int				setup_redir(t_cmd_data *cmd);
int				redir_out(int fd);
int				redir_in(int fd);

#endif
