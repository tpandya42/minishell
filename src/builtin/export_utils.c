/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:45:42 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/07 21:45:44 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_identifier_good(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	if (!(ft_isalpha((unsigned char)s[0]) || s[0] == '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=' && s[i] != '+')
	{
		if (!(ft_isalnum((unsigned char)s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	key_len_in_entry(const char *entry)
{
	int	i;

	i = 0;
	while (entry[i] && entry[i] != '=')
		i++;
	return (i);
}

int	env_count(char **envp)
{
	int	count;

	count = 0;
	while (envp && envp[count])
		count++;
	return (count);
}

int	env_find_index(char **envp, const char *key)
{
	int	i;
	int	klen;

	if (!envp || !key)
		return (-1);
	klen = (int)ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (key_len_in_entry(envp[i]) == klen && ft_strncmp(envp[i], key,
				klen) == 0)
			return (i);
		i++;
	}
	return (-1);
}

const char	*entry_value_ptr(const char *entry)
{
	int	k;

	k = key_len_in_entry(entry);
	if (entry[k] == '=')
		return (entry + k + 1);
	return ("");
}
