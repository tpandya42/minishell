/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:33:32 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/15 15:39:36 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <libgen.h>

void	restore_std(t_program *program)
{
	dup2(program->fd_in_orig, STDIN_FILENO);
	dup2(program->fd_out_orig, STDOUT_FILENO);
}

t_redir_type	map_type(t_toktype token_type)
{
	if (token_type == REDIR_IN)
		return (RED_IN);
	if (token_type == REDIR_OUT)
		return (RED_OUT);
	if (token_type == APPEND)
		return (RED_APPEND);
	if (token_type == HEREDOC)
		return (RED_HERE_DOC);
	return (RED_UNKNOW);
}

t_redir	*create_redir_node(char *target, enum e_redir_type type, bool quoted)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->target = ft_strdup(target);
	if (!new_redir->target)
	{
		free(new_redir);
		perror("ft_strdup failed redir target");
		return (NULL);
	}
	new_redir->type = type;
	new_redir->fd = -1;
	new_redir->next = NULL;
	new_redir->quoted = quoted;
	return (new_redir);
}

void	add_redir(t_redir **list, t_redir *new_redir)
{
	t_redir	*tmp;

	if (!new_redir)
		return ;
	if (!*list)
	{
		*list = new_redir;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_redir;
}

int	open_redir_filename(t_redir *redir)
{
	int		open_flags;

	if (redir->type == RED_IN)
		open_flags = O_RDONLY;
	else if (redir->type == RED_OUT)
		open_flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (redir->type == RED_APPEND)
		open_flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		return (1);
	if (redir->type == RED_IN && access(redir->target, F_OK) != 0)
	{
		perror(redir->target);
		return (1);
	}
	redir->fd = open(redir->target, open_flags, 0644);
	if (redir->fd < 0)
	{
		perror(redir->target);
		return (1);
	}
	return (0);
}
