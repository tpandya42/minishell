/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_03.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:46:40 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/07 21:46:41 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_replace(char ***penvp, int idx, char *kv)
{
	free((*penvp)[idx]);
	(*penvp)[idx] = kv;
	return (0);
}

int	env_extend(char ***penvp, char *kv)
{
	int		n;
	char	**new_env;
	int		i;

	n = env_count(*penvp);
	new_env = malloc(sizeof(char *) * (n + 2));
	if (!new_env)
	{
		free(kv);
		return (-1);
	}
	i = -1;
	while (++i < n)
		new_env[i] = (*penvp)[i];
	new_env[n] = kv;
	new_env[n + 1] = NULL;
	free(*penvp);
	*penvp = new_env;
	return (0);
}

int	env_update_append(char ***penvp, int idx, const char *key,
		const char *suffix)
{
	const char	*oldv;
	char		*joined;
	char		*kv;

	oldv = entry_value_ptr((*penvp)[idx]);
	joined = ft_strjoin((char *)oldv, (char *)suffix);
	if (!joined)
		return (-1);
	kv = make_kv_string(key, joined);
	free(joined);
	if (!kv)
		return (-1);
	free((*penvp)[idx]);
	(*penvp)[idx] = kv;
	return (0);
}

int	parse_append(const char *arg, int i, char **key_out, char **val_out)
{
	*key_out = ft_substr(arg, 0, i);
	*val_out = ft_strdup(arg + i + 2);
	if (*key_out && *val_out)
		return (0);
	return (-1);
}

int	parse_set(const char *arg, int i, char **key_out, char **val_out)
{
	*key_out = ft_substr(arg, 0, i);
	*val_out = ft_strdup(arg + i + 1);
	if (*key_out && *val_out)
		return (0);
	return (-1);
}
