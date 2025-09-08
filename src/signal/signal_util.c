/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 07:26:19 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/08 08:38:44 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ---  CTRL + C at PROMPT --- //
void	sigint_prompt(int signum)
{
	(void)signum;
	g_signal_value = SIGINT;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_prompt_heredoc(int signum)
{
	(void)signum;
	g_signal_value = SIGINT;
}

// --- CTRL + C DURING HEREDOC --- //
// Write a newline for visual feedback NEEDED
//130 to kill the child
void	sigint_heredoc(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	set_signal_handler(int signum, void (*handler)(int))
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(signum, &sa, NULL) == -1)
		perror("sigaction");
}
