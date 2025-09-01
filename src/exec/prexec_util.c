/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prexec_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 10:27:01 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/25 10:56:46 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_count_words(const char *str)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	if (!str)
		return (0);
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
		{
			if (!in_word)
			{
				count++;
				in_word = 1;
			}
		}
		else
			in_word = 0;
		str++;
	}
	return (count);
}

char	*remove_quotes(const char *str)
{
	char	*res;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	res = malloc(ft_strlen(str) + 1);//check where to free
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
		fprintf(stderr, BOLD RED "ambiguous redirect: '%s'\n" RESET,
			redir->target);
		free(expand);
		return (NULL);
	}
	return (expand);
}

char	*clean_target(t_redir *redir, char **expand)
{
	char	*clean;

	clean = remove_quotes(*expand);
	if (!clean || clean[0] == '\0')
	{
		fprintf(stderr, BOLD RED "ambiguous redirect: '%s'\n" RESET,
			redir->target);
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
		return (1);//invalid target check if smt to free
	expand = exp_redir(redir, envp_cpy, last_exit);
	if (!expand)
		return (1);
	clean = clean_target(redir, &expand);
	free (expand);
	if (!clean)
		return (1);
	if (ft_count_words(clean) > 1)
	{
		fprintf(stderr, BOLD RED "ambiguous redirect: '%s'\n" RESET,
			clean);
		free(clean);
		return (1);
	}
	free(redir->target);
	redir->target = clean;
	return (0);
}

void apply_redir(t_program *program, t_node *node)
{
    t_redir *redir = node->u_data.cmd.redir;

    while (redir)
    {
        if (redir->type == RED_HERE_DOC)
        {
            /* ensure proper delimiter + hd_expand flag */
            heredoc_normalize_delimiter(redir);
        }
        else
        {
            if (clean_redir_target(redir, program->envp_cpy, program->last_exit_status) != 0)
            {
                cleanup_program(program);
                exit(EXIT_FAILURE);
            }
        }
        redir = redir->next;
    }
}
