/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_io.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 17:29:30 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/12 08:30:12 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_tmp_heredoc(t_heredoc *hd, char **line)
{
	char	*to_write;

	to_write = hd->buf;
	if (hd->redir->hd_expand)
	{
		to_write = expand_token_text(hd->buf, hd->program->envp_cpy,
				hd->program->last_exit_status);
		free(hd->buf);
		hd->buf = NULL;
		if (line)
		{
			free (*line);
			*line = NULL;
		}
	}
	write(hd->pipefd[1], to_write, ft_strlen(to_write));
	if (hd->redir->hd_expand)
		free(to_write);
	*line = NULL;
	hd->buf = NULL;
}

//concatenate buf in tmp later maybe in fd
static void	process_line_heredoc(t_heredoc *hd, char *line)
{
	char	*old;
	char	*new;

	if (hd->buf)
	{
		old = hd->buf;
		new = ft_strjoin(hd->buf, line);
		free (old);
		hd->buf = new;
	}
	else
		hd->buf = ft_strdup(line);
	old = hd->buf;
	new = ft_strjoin(hd->buf, "\n");
	free (old);
	hd->buf = new;
	free (line);
}

// Process the line and expand if needed
void	expand_heredoc(t_program *program, char **buf)
{
	char	*expanded;

	expanded = expand_token_text(*buf, program->envp_cpy,
			program->last_exit_status);
	free(*buf);
	*buf = expanded;
}

static void	exit_ctrlc_heredoc(t_heredoc *hd, char **line)
{
	if (line && *line)
	{
		free (*line);
		*line = NULL;
	}
	if (hd->buf && hd->buf)
	{
		free (hd->buf);
		hd->buf = NULL;
	}
	close_fd(&hd->pipefd[1]);
	exit(130);
}

int	read_heredoc(t_heredoc *hd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_signal_value == SIGINT)
				exit_ctrlc_heredoc(hd, &line);
			if (hd->buf)
				write_tmp_heredoc(hd, &line);
			print_warn_ctrl_d(hd->current_line, hd->delimiter);
			free_hd(hd);
			break ;
		}
		if (is_delim_line(line, hd->redir->target))
		{
			free(line);
			break ;
		}
		process_line_heredoc(hd, line);
		hd->current_line++;
	}
	return (hd->current_line);
}
