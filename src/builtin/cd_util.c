/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_util.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:04:53 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/19 16:53:05 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_old_dest(char *old_pwd, char *dest_path)
{
	free(old_pwd);
	free(dest_path);
}

int	handle_cwd_error(char *dest_path)
{
	fprintf(stderr, RED BOLD "cd: getcwd error to get path\n" RESET);
	if (dest_path)
		free(dest_path);
	return (1);
}

static void	update_paths(t_program *program, char *old_pwd, char *new_path)
{
	update_env_var(program, "OLDPWD", old_pwd);
	update_env_var(program, "PWD", new_path);
}

void	update_free_paths(t_program *program, 
	char *old_pwd, char *new_cwd, char *dest_path)
{
	update_paths(program, old_pwd, new_cwd);
	if (old_pwd)
		free(old_pwd);
	if (new_cwd)
		free(new_cwd);
	if (dest_path)
		free(dest_path);
}

int	handle_env_path(t_program *program, char *key, char **dest_path)
{
	char	*tmp_path;

	tmp_path = find_env_value(program->envp_cpy, key);
	if (!tmp_path)
	{
		fprintf(stderr, "cd: %s not set\n", key);
		if (*dest_path)
			free(*dest_path);
		return (1);
	}
	*dest_path = ft_strdup(tmp_path);
	if (!*dest_path)
	{
		fprintf(stderr, RED BOLD "cd: memory error in handl env path\n" RESET);
		return (1);
	}
	return (0);
}
