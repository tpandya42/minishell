/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 22:44:22 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/15 16:59:27 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**ft_dup_env(char **envp)
{
	int		i;
	char	**envp_cpy;

	i = 0;
	while (envp[i])
		i++;
	envp_cpy = malloc(sizeof(char *) * (i + 1));
	if (!envp_cpy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		envp_cpy[i] = ft_strdup(envp[i]);
		if (!envp_cpy[i])
		{
			envp_cpy[i] = NULL;
			free_array(envp_cpy);
			return (NULL);
		}
		i++;
	}
	envp_cpy[i] = NULL;
	return (envp_cpy);
}

static int	init_fd_origin(t_program *program, int orig_fd, const char *err_msg)
{
	int	new_fd;

	new_fd = dup(orig_fd);
	if (new_fd == -1)
	{
		ft_print_error(err_msg);
		ft_print_error("\n");
		program->last_exit_status = 1; 
	}
	return (new_fd);
}

static void	handle_init_error(t_program *program)
{
	ft_print_error("Fatal initialization error\n");
	cleanup_program(program);
	exit(1);
}

void	init_program(t_program *program, char **envp)
{
	program->line = NULL;
	program->envp = envp;
	program->envp_cpy = ft_dup_env(envp);
	program->root = NULL;
	program->token_list = NULL;
	program->fd_in_orig = init_fd_origin(program,
			STDIN_FILENO, "dup failed for STDIN");
	program->fd_out_orig = init_fd_origin(program,
			STDOUT_FILENO, "dup failed for STDOUT");
	program->last_exit_status = 0;
	if (!program->envp_cpy)
	{
		malloc_error(program);
		exit (1);
	}
	if (!program->envp_cpy || program->fd_in_orig == -1
		|| program->fd_out_orig == -1)
		handle_init_error(program);
	if (tcgetattr(STDIN_FILENO, &program->orig_termios) == -1)
	{
		program->last_exit_status = 1;
		perror("tcgetattr");
		handle_init_error(program);
	}
}
