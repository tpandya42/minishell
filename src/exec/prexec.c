/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prexec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 08:41:11 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/29 14:23:57 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	free_partial_arr(char **arr, int allocated_i)//check vs free_arr
{
	if (!arr)
		return ;
	while (allocated_i > 0)
	{
		free (arr[allocated_i - 1]);
		allocated_i--;
	}
	free (arr);
}

static int	count_tokens(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		count++;
		token = token->next;
	}
	return (count);
}

static int	is_operator_token(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == PIPE ||
		token->type == AND ||
		token->type == OR ||
		token->type == SEMICOLON)
		return (1);
	return (0);
}

// This function counts only the command tokens for the argv array //
// it skips redirection tokens and their arguments.   
static int	count_cmd_tokens(t_token *token)
{
	int	count;

	count = 0;
	while (token && !is_operator_token(token))
	{
		if (token->type == REDIR_IN || token->type == REDIR_OUT ||
			token->type == APPEND || token->type == HEREDOC)
		{
			if (token->next)
				token = token->next->next;
			else
				token = token->next;
		}
		else
		{
			count++;
			token = token->next;
		}
	}
	return (count);
}


// ------------------------------------------------------------------ //
// This function copies the command tokens into the argv array.       //
// It skips redirection tokens and their arguments.                   //
// ------------------------------------------------------------------ //
static int	cpy_token_str(char **argv, t_token *token)
{
	t_token	*current_token;
	int		i;

	current_token = token;
	i = 0;
	while (current_token && !is_operator_token(current_token))
	{
		if (current_token->type == REDIR_IN || current_token->type == REDIR_OUT ||
			current_token->type == APPEND || current_token->type == HEREDOC)
		{
			if (current_token->next)
				current_token = current_token->next->next;
			else
				current_token = current_token->next;
		}
		else
		{
			argv[i] = ft_strdup(current_token->txt);
			if (!argv[i])
			{
				perror ("Failed ft_strdup token to argv");//check
				free_partial_arr(argv, i);
				return (1);
			}
			current_token = current_token->next;
			i++;
		}
	}
	argv[i] = NULL;
	return (0);
}

char	**token_to_argv(t_token *token)
{
	char	**argv;
	int		nb_token;

	nb_token = count_cmd_tokens(token);
	argv = malloc(sizeof(char *) * (nb_token + 1));//check where to free
	if (!argv)
		malloc_error();//check if exit or NULL (may be NULL)
	if (cpy_token_str(argv, token) != 0)
	{
		free(argv);
		fprintf(stderr, BOLD CYAN "copy_token_strings FAILED\n" RESET);//TEST
		return (NULL);
	}
	return (argv);
}

void	setup_cmd_arg(t_program *program, t_node *node)
{
	// fprintf(stderr, CYAN "Setting up command arguments...\n" RESET);//debug
	DEBUG_PRINT(CYAN "Setting up command arguments...\n" RESET);//debug
	node->u_data.cmd.argv = token_to_argv(node->u_data.cmd.tokens);
	if (!node->u_data.cmd.argv)
	{
		perror("Failed to create token args");//check msg
		cleanup_program(program);
		exit(EXIT_FAILURE);
	}
	DEBUG_PRINT(GREEN "Command arguments successfully created.\n" RESET);//debug
}


void	pre_execution(t_program *program, t_node *node)
{
	if (!node)
		return ;
	if (node->type == OPERATOR)
	{
		pre_execution(program, node->u_data.op.left);
		pre_execution(program, node->u_data.op.right);
	}
	else if (node->type == COMMAND)
	{
		DEBUG_PRINT(stderr, YELLOW BOLD "Pre-executing COMMAND node.\n" RESET); //debug
		// fprintf(stderr, YELLOW BOLD "Pre-executing COMMAND node.\n" RESET); //debug
		setup_cmd_arg(program, node);
		apply_redir(program, node);
		node->u_data.cmd.env = program->envp;
		// Removed duplicate initialization of fd_in and fd_out as they're already
		// initialized in init_cmd_node
		if (node->u_data.cmd.argv[0] && is_builtin(node->u_data.cmd.argv[0]))
			node->u_data.cmd.cmd_type = BUILTIN;
		else
			node->u_data.cmd.cmd_type = EXECUTABLE;
	}
}
