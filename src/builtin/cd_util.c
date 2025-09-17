/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_util.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:04:53 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/12 08:24:49 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_cwd_error(char *dest_path)
{
	(void)dest_path;
	perror("cd");
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

void	print_cd_not_set(const char *key)
{
	write(STDERR_FILENO, "cd: ", 4);
	write(STDERR_FILENO, key, ft_strlen(key));
	write(STDERR_FILENO, " not set\n", 9);
}

int	handle_env_path(t_program *program, char *key, char **dest_path)
{
	char	*tmp_path;

	tmp_path = find_env_value(program->envp_cpy, key);
	if (!tmp_path)
	{
		print_cd_not_set(key);
		if (*dest_path)
			free(*dest_path);
		return (1);
	}
	*dest_path = ft_strdup(tmp_path);
	if (!*dest_path)
	{
		perror("cd");
		return (1);
	}
	return (0);
}
