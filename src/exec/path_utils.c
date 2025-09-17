/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:17:51 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/07 23:17:53 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_executable(char *path)
{
	if (access(path, X_OK) == 0)
		return (1);
	return (0);
}

char	*find_in_path_dirs(char **dir, char *argv)
{
	int		i;
	char	*file_path;
	char	*final_path;

	i = 0;
	while (dir[i])
	{
		file_path = create_path(dir[i], argv);
		if (!file_path)
			return (free_prev_dir(dir, i));
		final_path = check_path(dir, file_path, i);
		if (final_path)
			return (final_path);
		free(file_path);
		free(dir[i]);
		i++;
	}
	free(dir);
	return (NULL);
}
