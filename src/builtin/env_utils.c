/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:08:10 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/19 16:58:31 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_env_entry(const char *key, const char *value)
{
	char	*tmp_str;
	char	*new_entry;

	tmp_str = ft_strjoin(key, "=");
	if (!tmp_str)
		return (NULL);
	new_entry = ft_strjoin(tmp_str, value);
	free(tmp_str);
	return (new_entry);
}

static int	count_envp(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

static void	add_env_var(t_program *program, const char *key, const char *value)
{
	char	**envp;
	char	**new_envp;
	int		count;
	int		i;

	envp = program->envp_cpy;
	count = count_envp(envp);
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return ;
	i = 0;
	while (envp[i])
	{
		new_envp[i] = envp[i];
		i++;
	}
	new_envp[count] = create_env_entry(key, value);
	if (!new_envp[count])
	{
		free_array(new_envp);
		return ;
	}
	new_envp[count + 1] = NULL;
	free(program->envp_cpy);
	program->envp_cpy = new_envp;
}

void	update_env_var(t_program *program, const char *key, const char *value)
{
	char	**envp;
	char	*new_entry;
	int		i;
	int		len;

	if (!program || !program->envp_cpy || !key || !value)
		return ;
	envp = program->envp_cpy;
	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
		{
			new_entry = create_env_entry(key, value);
			if (!new_entry)
				return ;
			free(envp[i]);
			envp[i] = new_entry;
			return ;
		}
		i++;
	}
	add_env_var(program, key, value);
}
