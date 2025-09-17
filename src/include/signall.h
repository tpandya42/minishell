/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signall.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 11:39:00 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/15 07:17:42 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALL_H
# define SIGNALL_H

# include "minishell.h"

extern sig_atomic_t	g_signal_value;

// Public signal handler functions
void				sigint_prompt(int signum);
void				sigint_prompt_heredoc(int signum);
void				sigint_heredoc(int signum);

// Public signal control functions
void				set_signal_handler(int signum, void (*handler)(int));
void				set_signal_prompt(int in_heredoc);
void				set_signal_child_process(void);
void				set_signal_heredoc(void);
void				sigint_parent_waiting(int signum);

#endif
