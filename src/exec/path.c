/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:13:58 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/12 15:18:15 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_path(void)
{
	char	*path;
	char	**dir;

	path = getenv("PATH");
	if (!path)
		return (NULL);
	dir = ft_split(path, ':');
	return (dir);
}

char	*create_path(char *dir, char *argv)
{
	char	*each_path;
	char	*file_path;

	each_path = ft_strjoin(dir, "/");
	if (!each_path)
	{
		free(dir);
		return (NULL);
	}
	file_path = ft_strjoin(each_path, argv);
	free(each_path);
	if (!file_path)
		return (NULL);
	return (file_path);
}

char	*check_path(char **dir, char *file_path, int i)
{
	if (access(file_path, F_OK) == 0)
	{
		while (dir[i])
		{
			free(dir[i]);
			i++;
		}
		free(dir);
		return (file_path);
	}
	return (NULL);
}

char	*free_prev_dir(char **dir, int i)
{
	while (dir[i])
	{
		free(dir[i]);
		i++;
	}
	free(dir);
	return (NULL);
}

char	*find_path(char *argv)
{
	char	**dir;

	if (argv && ft_strchr(argv, '/'))
	{
		if (is_executable(argv))
			return (ft_strdup(argv));
		return (NULL);
	}
	dir = get_path();
	if (!dir)
		return (NULL);
	return (find_in_path_dirs(dir, argv));
}
