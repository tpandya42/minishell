/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_array.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:54:19 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/23 12:42:51 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// void	free_array(char **arr, int j)
// {
// 	if (!arr)
// 		return ;
// 	while (j > 0)
// 	{
// 		free (arr[j - 1]);
// 		j--;
// 	}
// 	free (arr);
// }

void	free_array(char **arr)
{
	int	i;

	if (!arr) 
		return ;
	i = 0;
	while (arr[i])
	{
		if (arr[i])
			free(arr[i]);
		i++;
	}
	free(arr);
}
