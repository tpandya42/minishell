/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 08:42:03 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/16 07:45:26 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*token_parser_input(char *line, t_program *program)
{
	t_token	*token_list;
	t_token	*parser_tokens;
	t_node	*root;

	token_list = lex(line, program);
	if (!token_list || !validate_pipe_edges(token_list, program))
	{
		program->token_list = NULL;
		return (NULL);
	}
	expand(token_list, program->envp_cpy, program->last_exit_status);
	parser_tokens = token_list;
	root = parse(program, parser_tokens);
	if (!root)
	{
		free_token(token_list);
		program->token_list = NULL;
		return (NULL);
	}
	free_token_list(program);
	return (root);
}

static int	execute_with_signal(t_program *program, t_node *root)
{
	set_signal_handler(SIGINT, sigint_parent_waiting);
	program->last_exit_status = execution(program, root, false);
	if (g_signal_value == SIGINT)
	{
		program->last_exit_status = 130;
		g_signal_value = 0;
	}
	return (program->last_exit_status);
}

static void	set_exit_succes(t_program *program)
{
	program->last_exit_status = 0;
	return ;
}

static void	process_cmdline(t_program *program, char *line)
{
	t_node	*root;

	if (!line)
	{
		set_exit_succes(program);
		return ;
	}
	if (!*line)
	{
		program->last_exit_status = 0;
		free(line);
		return ;
	}
	add_history(line);
	root = token_parser_input(line, program);
	program->root = root;
	free(line);
	if (program->root)
	{
		pre_execution(program, root);
		execute_with_signal(program, root);
		free_ast_tokens(program);
	}
	free_token_list(program);
}

int	main(int argc, char **argv, char **envp)
{
	char		*prompt;
	t_program	program;

	(void)argc;
	(void)argv;
	init_program(&program, envp);
	prompt = BOLD GREEN "🐶🥕 Milanshell >" RESET;
	set_signal_prompt(0);
	while (1)
	{
		program.line = readline(prompt);
		if (!program.line && isatty(STDIN_FILENO))
		{
			printf(BLUE "exit\n" RESET);
			break ;
		}
		process_cmdline(&program, program.line);
		set_signal_prompt(0);
	}
	cleanup_program(&program);
	return (program.last_exit_status);
}
