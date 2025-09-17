/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_01.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:46:17 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/07 21:46:18 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_set(char ***penvp, const char *key, const char *value)
{
	int		idx;
	char	*kv;

	if (!penvp || !key)
		return (-1);
	kv = make_kv_string(key, value);
	if (!kv)
		return (-1);
	idx = env_find_index(*penvp, key);
	if (idx >= 0)
		return (env_replace(penvp, idx, kv));
	return (env_extend(penvp, kv));
}

int	env_append(char ***penvp, const char *key, const char *suffix)
{
	int	idx;

	if (!penvp || !key || !suffix)
		return (-1);
	idx = env_find_index(*penvp, key);
	if (idx >= 0)
		return (env_update_append(penvp, idx, key, suffix));
	return (env_set(penvp, key, suffix));
}

int	parse_export_arg(const char *arg, char **key_out, char **val_out,
		t_export_mode *mode_out)
{
	int	i;

	*key_out = NULL;
	*val_out = NULL;
	*mode_out = EXP_MARK;
	if (!arg || !*arg)
		return (-1);
	i = 0;
	while (arg[i] && arg[i] != '=' && arg[i] != '+')
		i++;
	if (arg[i] == '+' && arg[i + 1] == '=')
	{
		*mode_out = EXP_APPEND;
		return (parse_append(arg, i, key_out, val_out));
	}
	if (arg[i] == '=')
	{
		*mode_out = EXP_SET;
		return (parse_set(arg, i, key_out, val_out));
	}
	*key_out = ft_strdup(arg);
	if (*key_out)
		return (0);
	return (-1);
}
