/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 16:37:46 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/12 08:29:57 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "minishell.h"

typedef struct s_heredoc
{
	t_redir		*redir;
	t_program	*program;
	char		*buf;
	int			pipefd[2];
	int			current_line;
	char		*delimiter;
}				t_heredoc;

//  Main heredoc functions  //
int				heredoc_prepare(t_redir *redir, t_program *program);
int				read_heredoc(t_heredoc *hd);

//      Heredoc helpers     //
int				is_delim_line(const char *line, const char *delim);
void			heredoc_normalize_delimiter(t_redir *redir);
void			init_hd(t_heredoc *hd, t_redir *redir, t_program *program,
					int pipefd[2]);
void			free_hd(t_heredoc *hd);
char			*strip_outer_quotes(const char *s);
void			expand_heredoc(t_program *program, char **buf);

#endif
