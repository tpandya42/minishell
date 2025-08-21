/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:59:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/20 15:48:41 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Redirects STDIN_FILENO to a specified fd
//  */
// int	redir_input(int fd)
// {
// 	int	fd_dup;

// 	fd_dup = dup2(fd, STDIN_FILENO);
// 	if (fd_dup == -1)
// 	{
// 		perror (BOLD RED "Dup2 in redir_input: Bad file descriptor" RESET);
// 		close_fd (fd);
// 		return (1);
// 	}
// 	close_fd(fd);
// 	return (0);
// }
// /**
//  * @brief Redirects STDOUT_FILENO to a specified fd
//  */
// int	redir_output(int fd)
// {
// 	int	fd_dup;

// 	fd_dup = dup2(fd, STDOUT_FILENO);
// 	if (fd_dup == -1)
// 	{
// 		perror (BOLD RED "Dup2 in redir_output: Bad file descriptor" RESET);
// 		close_fd(fd);
// 		return (1);
// 	}
// 	close_fd(fd);
// 	return (0);
// }

// /**
//  * @brief Sets up both standard input and standard 
//  * output redirections for a command.
//  */
// int	setup_redir(int fd_in, int fd_out, t_fd_dup *dup)
// {
// 	int	fd_in_dup;
// 	int	fd_out_dup;

// 	fd_in_dup = redir_input(fd_in);
// 	if (fd_in_dup < 0)
// 	{
// 		perror(BOLD RED "Failed redir_input" RESET);
// 		return (-1);
// 	}
// 	fd_out_dup = redir_output(fd_out);
// 	if (fd_out_dup < 0)
// 	{
// 		close_fd(fd_in_dup);
// 		perror(BOLD RED "Failed redirection OUTPUT" RESET);
// 		return (-1);
// 	}
// 	dup->input_dup = fd_in_dup;
// 	dup->output_dup = fd_out_dup;
// 	return (0);
// }
