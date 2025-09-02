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
	// Set up stdin from pipe if appropriate
	if (cmd->fd_in == STDIN_FILENO && cmd->pipefd[0] >= 0)
	{
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
}

// decides cmd execution
int	handle_cmd_exec(t_program *program, t_node *node, bool is_pipe_child)
{
	int			status;
	char		*cmd_name;
	t_cmd_data	*cmd;

	if (!node || !node->u_data.cmd.argv)
		return (1);
	// Remove quotes from command name for builtins and external commands
	char *cmd_name_unquoted = strip_outer_quotes(node->u_data.cmd.argv[0]);
	cmd_name = cmd_name_unquoted ? cmd_name_unquoted : node->u_data.cmd.argv[0];//change ternary
	if (is_pipe_child && node->u_data.cmd.redir == NULL)
		cmd = &node->u_data.cmd;
	if (is_operator_str(cmd_name))
	{
		fprintf(stderr, RED BOLD
			"Syntax error near unexpected token `%s`\n" RESET, cmd_name);
		return (1);//syntax error can be 2?
	}
	if (is_pipe_child)
	{
		// Process redirections if there are any
		if (node->u_data.cmd.redir)
		{
			if (process_redir(&node->u_data.cmd, program) != 0)
				exit(EXIT_FAILURE);
		}
		
		// Always set up final file descriptors for pipes
		set_final_fds(&node->u_data.cmd);
		
		if (is_builtin(cmd_name))
		{
			status = execute_builtin(program, node, true);
			if (cmd_name_unquoted)
			{
				free(cmd_name_unquoted);
			}
			exit(status);
		}
		else
		{
			// For external commands, update argv[0] to unquoted
			if (cmd_name_unquoted)
			{
				free(node->u_data.cmd.argv[0]);
				node->u_data.cmd.argv[0] = cmd_name_unquoted;
			}
			exec_cmd_inchild(node);  // Direct execution without fork
			// Should not reach here - exec_cmd_inchild should not return
			perror("Error executing command in pipe");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (is_builtin(cmd_name))
		{
			cmd = &node->u_data.cmd;
			if (cmd->redir) 
			{
				if (process_redir(cmd, program) == 0) {
					setup_redir(cmd);
				}
			}
			else
			{
				setup_redir(cmd);
			}
			status = execute_builtin(program, node, false);
			if (cmd_name_unquoted)
				free(cmd_name_unquoted);
			if (cmd->redir) {
				// Clean up file descriptors before restoring standard input/output
				cleanup_fds(cmd);
				restore_std(program);
			}
			program->last_exit_status = status;
			return (status);
		}
		else
		{
			// For external commands, update argv[0] to unquoted
			if (cmd_name_unquoted) {
				free(node->u_data.cmd.argv[0]);
				node->u_data.cmd.argv[0] = cmd_name_unquoted;
			}
			if (process_redir(&node->u_data.cmd, program) != 0)
				return 1;
			// Setup redirections before executing the command
			setup_redir(&node->u_data.cmd);
			int status = exec_cmd_nopipe(program, node);
			// Clean up file descriptors before restoring standard input/output
			cleanup_fds(&node->u_data.cmd);
			restore_std(program);
			return status;
		}
	}
	return (1);//added only to compile
}

int	execution(t_program *program, t_node *node, bool is_pipe_child)
{
	int	status;

	if (!node)
	{
		program->last_exit_status = 0;
		return (0);
	}
	if (node->type == COMMAND) 
	{
		status = handle_cmd_exec(program, node, is_pipe_child);
	}
	else if (node->type == OPERATOR)
	{
		status = handle_operator(program, node, is_pipe_child);
	}
	else
	{
		fprintf(stderr, "Error: unknown node type for execution\n");
		status = 1;
	}
	program->last_exit_status = status;
	return (status);
}

