/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 17:30:19 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/07 18:41:00 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//  Frees the delimiter field in the heredoc struct.
static void	free_hd_delimiter(t_heredoc *hd)
{
	if (hd->delimiter)
	{
		free(hd->delimiter);
		hd->delimiter = NULL;
	}
}

//  Frees all dynamically allocated memory in the heredoc struct.
void	free_hd(t_heredoc *hd)
{
	if (hd->buf)
	{
		free(hd->buf);
		hd->buf = NULL;
	}
	free_hd_delimiter(hd);
}

void	init_hd(t_heredoc *hd, t_redir *redir,
	t_program *program, int pipefd[2])
{
	hd->redir = redir;
	hd->program = program;
	hd->buf = NULL;
	hd->pipefd[0] = pipefd[0];
	hd->pipefd[1] = pipefd[1];
	hd->current_line = 1;
	hd->delimiter = ft_strdup(redir->target);
}
