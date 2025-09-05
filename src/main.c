/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 08:42:03 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/29 13:00:25 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*token_parser_input(char *line, t_token **token_out, t_program *program)
{
	t_token	*token_list;
	t_token	*parser_tokens;
	t_node	*root;

	token_list = lex(line, ' ');
	if (!token_list)
	{
		fprintf(stderr, BOLD RED "Lexing failed or empty imput\n" RESET);
		*token_out = NULL;
		return (NULL);
	}
	expand(token_list, program->envp_cpy, program->last_exit_status);
	parser_tokens = token_list;
	root = parse(parser_tokens);
	if (!root)
	{
		fprintf(stderr, RED BOLD "Parsing failed\n" RESET);
		free_token(token_list);
		*token_out = NULL;
		return (NULL);
	}
	*token_out = token_list;
	return (root);
}

static void	process_cmdline(t_program *program, char *line)
{
	t_token	*token_list;
	t_node	*root;

	if (!line)
		return ;
	if (!*line)
	{
		free(line);
		return ;
	}
	add_history(line);
	root = token_parser_input(line, &program->token_list, program);
	program->root = root;
	free(line);
	if (!program->root)
	{
		fprintf(stderr, RED "Parsing failed\n" RESET);
		return ;
	}
	// DEBUG removed//TEST
	// DEBUG removed//test
	pre_execution(program, root);
	program->last_exit_status = execution(program, root, false);
	free_ast_tokens(program);
}

int	main(int argc, char **argv, char **envp)
{
	char		*prompt;
	t_program	program;

	(void) argc;//check if needed
	(void) argv;//check if needed

	init_program(&program, envp);
	prompt = BOLD GREEN "🐶🥕 Milanshell >" RESET;
	set_signal_prompt();
	while (1)
	{
		program.line = readline(prompt);
		if (!program.line && isatty(STDIN_FILENO)) //if issaty returns 0 is in an fd
		{
			printf(BLUE "exit\n" RESET);
			break ;
		}
		process_cmdline(&program, program.line);
	}
	cleanup_program(&program);
	// fprintf(stderr, MAGENTA BOLD "last program status: %d\n" RESET, program.last_exit_status);//new TEST
	// DEBUG removed//TEST
	return (program.last_exit_status);
}
