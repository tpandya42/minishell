/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 22:35:39 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/09 22:35:39 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(const char *var, char **envp_cpy)
{
	int		i;
	size_t	var_len;

	i = 0;
	var_len = ft_strlen(var);
	while (envp_cpy && envp_cpy[i])
	{
		if (ft_strncmp(envp_cpy[i], var, var_len) == 0
			&& envp_cpy[i][var_len] == '=')
			return (ft_strdup(envp_cpy[i] + var_len + 1));
		i++;
	}
	return (ft_strdup(""));
}

char	*status_exp(char c, int last_exit)
{
	char	*res;

	if (c == '?')
		res = ft_itoa(last_exit);
	else if (c == '$')
		res = ft_itoa(getpid());
	else
		res = ft_strdup("");
	return (res);
}

char	*expand_token_helper(char *result, char *pos)
{
	char	*temp;
	char	tmp[2];

	tmp[0] = *pos;
	tmp[1] = '\0';
	temp = ft_strjoin(result, tmp);
	free(result);
	result = temp;
	return (result);
}

char	*expand_token_helper_01(char **pos, char **envp_cpy)
{
	size_t	var_len;
	char	*var_name;
	char	*val;

	var_len = 0;
	while ((*pos)[1 + var_len] && (ft_isalnum((*pos)[1 + var_len]) || (*pos)[1
			+ var_len] == '_'))
		var_len++;
	if (var_len == 0)
	{
		val = ft_strdup("$");
		(*pos)++;
	}
	else
	{
		var_name = ft_substr(*pos + 1, 0, var_len);
		val = get_env_value(var_name, envp_cpy);
		free(var_name);
		*pos += var_len + 1;
	}
	return (val);
}
