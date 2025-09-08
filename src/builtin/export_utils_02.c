/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_02.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:46:32 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/07 21:46:34 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**dup_envp(char **envp)
{
	int		n;
	int		i;
	char	**copy;

	n = env_count(envp);
	copy = (char **)malloc(sizeof(char *) * (n + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < n)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			while (--i >= 0)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[n] = NULL;
	return (copy);
}

void	sort_envp(char **envp)
{
	int		i;
	int		j;
	int		n;
	char	*tmp;

	n = env_count(envp);
	i = 0;
	while (i < n)
	{
		j = i + 1;
		while (j < n)
		{
			if (ft_strcmp(envp[i], envp[j]) > 0)
			{
				tmp = envp[i];
				envp[i] = envp[j];
				envp[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

char	*make_kv_string(const char *key, const char *value)
{
	char	*tmp;
	char	*kv;

	if (!value)
		value = "";
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	kv = ft_strjoin(tmp, value);
	free(tmp);
	return (kv);
}

void	export_error_identifier(const char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

int	env_mark_export(char ***penvp, const char *key)
{
	if (!penvp || !key)
		return (-1);
	if (env_find_index(*penvp, key) >= 0)
		return (0);
	return (env_set(penvp, key, ""));
}
