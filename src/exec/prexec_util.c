/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prexec_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 10:27:01 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/12 07:27:01 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(const char *str)
{
	char	*res;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '"')
			res[j++] = str[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}

char	*exp_redir(t_redir *redir, char **envp_cpy, int last_exit)
{
	char	*expand;

	expand = expand_token_text(redir->target, envp_cpy, last_exit);
	if (!expand || expand[0] == '\0')
	{
		ft_print_error("syntax error: ambiguous redirect");
		free(expand);
		return (NULL);
	}
	return (expand);
}

char	*clean_target(t_redir *redir, char **expand)
{
	char	*clean;

	(void)redir;
	clean = remove_quotes(*expand);
	if (!clean || clean[0] == '\0')
	{
		ft_print_error("syntax error: ambiguous redirect");
		free(clean);
		return (NULL);
	}
	return (clean);
}

int	clean_redir_target(t_redir *redir, char **envp_cpy, int last_exit)
{
	char	*expand;
	char	*clean;

	if (!redir || !redir->target)
		return (1);
	expand = exp_redir(redir, envp_cpy, last_exit);
	if (!expand)
		return (1);
	clean = clean_target(redir, &expand);
	free(expand);
	if (!clean)
		return (1);
	if (ft_count_words(clean) > 1)
	{
		ft_print_error("syntax error: ambiguous redirect");
		free(clean);
		return (1);
	}
	free(redir->target);
	redir->target = clean;
	return (0);
}

void	apply_redir(t_program *program, t_node *node)
{
	t_redir	*redir;

	redir = node->u_data.cmd.redir;
	while (redir)
	{
		if (redir->type == RED_HERE_DOC)
			heredoc_normalize_delimiter(redir);
		else
		{
			if (clean_redir_target(redir, program->envp_cpy,
					program->last_exit_status) != 0)
			{
				cleanup_program(program);
				exit(EXIT_FAILURE);
			}
		}
		redir = redir->next;
	}
}
