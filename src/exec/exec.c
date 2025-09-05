/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:32:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/29 14:27:33 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Executes a cmd with execve
//doesn't fork
void	exec_cmd_inchild(t_node *node)
{
	char	*cmd_path;

	cmd_path = find_path (node->u_data.cmd.argv[0]);
	if (!cmd_path)
	{
		perror (BOLD RED "command_path not found" RESET);
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, node->u_data.cmd.argv, node->u_data.cmd.env);
	perror("Error: execve failed");
	free(cmd_path);
	exit(EXIT_FAILURE);
}

static int	handle_operator(t_program *program, t_node *node, bool is_pipe_child)//NEW
{
	int	left_status;
	int	right_status;

	if (node->u_data.op.type == PIPE)
		return (execute_pipeline(program, node));
	else if (node->u_data.op.type == AND)
	{
		left_status = execution(program, node->u_data.op.left, true);
		if (left_status == 0)
			return (execution(program, node->u_data.op.right, true));
		return (left_status);
	}
	// else if (node->u_data.op.type == OR)
	// {
	// 	left_status = execution(program, node->u_data.op.left);
	// 	if (left_status != 0)
	// 		return (execution(program, node->u_data.op.right));
	// 	return (left_status);
	// }
	// else if (node->u_data.op.type == SEMICOLON)
	// {
	// 	execution(program, node->u_data.op.left);
	// 	return (execution(program, node->u_data.op.right));
	// }
	fprintf(stderr, BOLD RED "Error: unknow type operand for execution\n" RESET);
	return (1);
}

int	is_operator_str(const char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, "|") == 0
		|| ft_strcmp(str, "&&") == 0
		|| ft_strcmp(str, "||") == 0
		|| ft_strcmp(str, ";") == 0)
		return (1);
	return (0);
}

// in child before executing cmd
// connect pipes if there is no redir
void	set_final_fds(t_cmd_data *cmd)
{
	// Debug logs removed

	// Set up stdin from pipe if appropriate
	if (cmd->fd_in == STDIN_FILENO && cmd->pipefd[0] >= 0)
	{
		// DEBUG removed
		if (dup2(cmd->pipefd[0], STDIN_FILENO) == -1)
		{
			perror("Error: dup2 failed for pipe input");
			exit(1);
		}
		close_fd(&cmd->pipefd[0]);
	}
	else if (cmd->fd_in >= 0 && cmd->fd_in != STDIN_FILENO)
	{
		// Handle explicit input redirection
		// DEBUG removed
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
		{
			perror("Error: dup2 failed for input redirection");
			exit(1);
		}
		close_fd(&cmd->fd_in);
	}

	// Set up stdout to pipe if appropriate
	if (cmd->fd_out == STDOUT_FILENO && cmd->pipefd[1] >= 0)
	{
		// DEBUG removed
		if (dup2(cmd->pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("Error: dup2 failed for pipe output");
			exit(1);
		}
		close_fd(&cmd->pipefd[1]);
	}
	else if (cmd->fd_out >= 0 && cmd->fd_out != STDOUT_FILENO)
	{
		// Handle explicit output redirection
		// DEBUG removed
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		{
			perror("Error: dup2 failed for output redirection");
			exit(1);
		}
		close_fd(&cmd->fd_out);
	}

	// Close any remaining pipe fds
	if (cmd->pipefd[0] >= 0)
		close_fd(&cmd->pipefd[0]);
	if (cmd->pipefd[1] >= 0)
		close_fd(&cmd->pipefd[1]);

	// DEBUG removed
}

// decides cmd execution

int	handle_cmd_exec(t_program *program, t_node *node, bool is_pipe_child)
{
	int			status;
	char		*cmd_name;
	t_cmd_data	*cmd;

	if (!node || !node->u_data.cmd.argv)
		return (1);

	// --- Unquote command name and replace argv[0] directly ---
	char *cmd_name_unquoted = strip_outer_quotes(node->u_data.cmd.argv[0]);
	if (cmd_name_unquoted) {
		free(node->u_data.cmd.argv[0]);
		node->u_data.cmd.argv[0] = cmd_name_unquoted;
	}
	cmd_name = node->u_data.cmd.argv[0]; // safe pointer to use everywhere

	if (is_pipe_child && node->u_data.cmd.redir == NULL)
		cmd = &node->u_data.cmd;

	if (is_operator_str(cmd_name))
	{
		fprintf(stderr, RED BOLD
			"Syntax error near unexpected token `%s`\n" RESET, cmd_name);
		return (2); // syntax error usually returns 2 in bash
	}

	if (is_pipe_child)
	{
		cmd = &node->u_data.cmd;

		// Process redirections if any
		if (cmd->redir && process_redir(cmd, program) != 0)
			exit(EXIT_FAILURE);

		// Set final file descriptors for pipes
		set_final_fds(cmd);

		if (is_builtin(cmd_name))
			exit(execute_builtin(program, node, true));
		else
		{
			exec_cmd_inchild(node); // should not return
			perror("Error executing command in pipe");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		cmd = &node->u_data.cmd;

		if (is_builtin(cmd_name))
		{
			if (cmd->redir)
			{
				if (process_redir(cmd, program) == 0)
					setup_redir(cmd);
			}
			else
				setup_redir(cmd);

			status = execute_builtin(program, node, false);

			if (cmd->redir)
			{
				cleanup_fds(cmd);
				restore_std(program);
			}
			program->last_exit_status = status;
			return status;
		}
		else
		{
			// External command
			if (process_redir(cmd, program) != 0)
				return 1;

			setup_redir(cmd);
			status = exec_cmd_nopipe(program, node);

			// Clean up redirections
			cleanup_fds(cmd);
			restore_std(program);
			return status;
		}
	}

	// unreachable, added only to satisfy compiler
	return 1;
}


int	execution(t_program *program, t_node *node, bool is_pipe_child)
{
	int	status;

	// fprintf(stderr, MAGENTA BOLD "About to dispatch a node execution\n" RESET);//test
	// DEBUG removed//debug
	if (!node)
	{
		program->last_exit_status = 0;
		// fprintf(stderr, MAGENTA BOLD "Last cmd updated: %d\n" RESET, program->last_exit_status);//test
		// DEBUG removed//test
		return (0);
	}
	if (node->type == COMMAND) 
	{
		// fprintf(stderr, MAGENTA BOLD "Executing a COMMAND node...\n" RESET); //debug
		// DEBUG removed //debug
		status = handle_cmd_exec(program, node, is_pipe_child);
		// fprintf(stderr, MAGENTA BOLD "Will be a cmd\n" RESET);//test
		// DEBUG removed//test
	}
	else if (node->type == OPERATOR)
	{
		// fprintf(stderr, MAGENTA BOLD "Executing an OPERATOR node...\n" RESET);//debug
		// DEBUG removed//debug
		status = handle_operator(program, node, is_pipe_child);
		// fprintf(stderr, MAGENTA BOLD "Will be a oprator\n" RESET);//test
		// DEBUG removed//test
	}
	else
	{
		fprintf(stderr, BOLD RED "Error: unknow type node for execution\n" RESET);
		status = 1;
	}
	program->last_exit_status = status;
	// fprintf(stderr, MAGENTA BOLD "Node execution finished with status: %d\n" RESET, status);//debug
	// DEBUG removed//debug
	return (status);
}

