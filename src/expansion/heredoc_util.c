/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 17:30:08 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/12 08:29:51 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*strip_outer_quotes(const char *s)
{
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (len >= 2 && ((s[0] == '\'' && s[len - 1] == '\'') 
			|| (s[0] == '"' && s[len - 1] == '"')))
		return (ft_substr(s, 1, len - 2));
	return (ft_strdup(s));
}

int	is_delim_line(const char *line, const char *delim)
{
	if (!line || !delim)
		return (0);
	return (ft_strcmp(line, delim) == 0);
}

/* ---- public helpers ---------------------------------------------------- */

/* normalize the delimiter: strip quotes and set expand flag */
// Use quoted field from redir struct
void	heredoc_normalize_delimiter(t_redir *redir)
{
	char	*clean;

	if (!redir || !redir->target)
		return ;
	redir->hd_expand = !redir->quoted;
	clean = strip_outer_quotes(redir->target);
	if (clean)
	{
		free(redir->target);
		redir->target = clean;
	}
}
