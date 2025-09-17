/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 07:25:18 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/15 07:18:02 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t	g_signal_value = 0;

// --- CONTEXT-SPECIFIC SIGNAL SETUP --- //
void	set_signal_prompt(int in_heredoc)
{
	if (in_heredoc)
		set_signal_handler(SIGINT, sigint_prompt_heredoc);
	else
		set_signal_handler(SIGINT, sigint_prompt);
	set_signal_handler(SIGQUIT, SIG_IGN);
}

void	sigint_parent_waiting(int signum)
{
	(void)signum;
	g_signal_value = SIGINT;
	write(1, "\n", 1);
}

void	set_signal_child_process(void)
{
	set_signal_handler(SIGINT, SIG_DFL);
	set_signal_handler(SIGQUIT, SIG_DFL);
}

void	set_signal_heredoc(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}
