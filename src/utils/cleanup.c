/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 12:20:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/16 07:36:55 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Cleaning after each cmd in cmd line
//free tmp resources per each cmd like root and token list
void	free_ast_tokens(t_program *program)
{
	if (program->token_list)
	{
		free_token(program->token_list);
		program->token_list = NULL;
	}
	if (program->root)
	{
		free_node(program->root);
		program->root = NULL;
	}
}

void	free_ast(t_program *program)
{
	free_node(program->root);
	program->root = NULL;
}

void	free_env_cpy(t_program *program)
{
	free_array(program->envp_cpy);
	program->envp_cpy = NULL;
}

//To centralized cleanup at the end of the program
//to finish the program
//readline does the frees the line
void	cleanup_program(t_program *program)
{
	if (!program)
		return ;
	if (program->root)
	{
		free_ast(program);
		program->root = NULL;
	}
	if (program->token_list)
	{
		free_token_list(program);
		program->token_list = NULL;
	}
	program->line = NULL;
	if (program->envp_cpy)
	{
		free_env_cpy(program);
		program->envp_cpy = NULL;
	}
	if (program->fd_in_orig != -1)
		close_fd(&program->fd_in_orig);
	if (program->fd_out_orig != -1)
		close_fd(&program->fd_out_orig);
}

void	malloc_error(t_program *program)
{
	safe_write(STDERR_FILENO, "Error\n", 6);
	cleanup_program(program); 
}
