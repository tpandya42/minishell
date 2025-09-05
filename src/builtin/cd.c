/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:04:46 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/29 14:30:31 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_dest_path(t_program *program, t_node *node, char **dest_path)
{
	if (!node->u_data.cmd.argv[1] 
		|| ft_strcmp(node->u_data.cmd.argv[1], "~") == 0)
		return (handle_env_path(program, "HOME", dest_path));
	else if (ft_strcmp(node->u_data.cmd.argv[1], "-") == 0)
		return (handle_env_path(program, "OLDPWD", dest_path));
	else
	{
		*dest_path = ft_strdup(node->u_data.cmd.argv[1]);
		if (!*dest_path)
		{
			fprintf(stderr, "cd: memory allocation error\n");
			return (1);
		}
		return (0);
	}
}

static int	get_old_pwd(char **old_pwd, char *dest_path)
{
	*old_pwd = getcwd(NULL, 0);
	if (!*old_pwd)
	{
		/* getcwd failed (for example, current dir removed). Print a warning
		 * but do not abort: we still attempt to chdir to the destination. */
		fprintf(stderr, RED BOLD "cd: getcwd error to get path\n" RESET);
		*old_pwd = NULL;
	}
	return (0);
}

static int	change_dir(char *dest_path)
{
	if (chdir(dest_path) == -1)
	{
		perror("Builtin error: cd");
		return (1);
	}
	return (0);
}

static int	update_final_paths(t_program *program,
	char *old_pwd, char *dest_path)
{
	char	*new_cwd;

	new_cwd = getcwd(NULL, 0);
	if (!new_cwd)
	{
		update_free_paths(program, old_pwd, NULL, dest_path);
		return (1);
	}
	update_free_paths(program, old_pwd, new_cwd, dest_path);
	return (0);
}

int	my_cd(t_program *program, t_node *node)
{
	char	*old_pwd;
	char	*dest_path;
	bool	print_path;

	print_path = false;
	// fprintf(stderr, MAGENTA BOLD "MY CD is about to be run\n" RESET);
	// DEBUG removed//DEBUG
	if (node->u_data.cmd.argv[1] 
		&& ft_strcmp(node->u_data.cmd.argv[1], "-") == 0)
		print_path = true;
	if (get_dest_path(program, node, &dest_path) != 0)
		return (1);
	if (get_old_pwd(&old_pwd, dest_path) != 0)
	{
		free(dest_path);
		return (1);
	}
	if (change_dir(dest_path) != 0)
	{
		update_free_paths(program, old_pwd, NULL, dest_path);
		return (1);
	}
	if (print_path)
		printf("%s\n", dest_path);
	return (update_final_paths(program, old_pwd, dest_path));
}
