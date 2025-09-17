/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:50:11 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/07 21:50:12 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	key_matches(const char *env_entry, const char *key)
{
	char	*equal_pos;
	size_t	key_len;

	equal_pos = ft_strchr(env_entry, '=');
	if (!equal_pos)
		return (0);
	key_len = equal_pos - env_entry;
	if (ft_strncmp(env_entry, key, key_len) == 0
		&& ft_strlen(key) == key_len)
		return (1);
	return (0);
}

static void	remove_from_env(char ***envp, const char *key)
{
	int	i;
	int	j;

	i = 0;
	if (!*envp || !key)
		return ;
	while ((*envp)[i])
	{
		if (key_matches((*envp)[i], key))
		{
			free((*envp)[i]);
			j = i;
			while ((*envp)[j])
			{
				(*envp)[j] = (*envp)[j + 1];
				j++;
			}
			i--;
		}
		i++;
	}
}

int	my_unset(t_program *program, t_node *node)
{
	int		i;
	char	**args;

	args = node->u_data.cmd.argv;
	if (!args || !args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		remove_from_env(&program->envp_cpy, args[i]);
		i++;
	}
	return (0);
}
