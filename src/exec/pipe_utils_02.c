/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_02.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:44:27 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/12 15:21:17 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_cmd_pipefds(t_cmd_data *cmd, int i, int n_cmds, int (*pipes)[2])
{
	cmd->pipefd[0] = -1;
	cmd->pipefd[1] = -1;
	if (i > 0 && !has_redir_in(cmd->redir))
		cmd->pipefd[0] = pipes[i - 1][0];
	if (i < n_cmds - 1 && !has_redir_out(cmd->redir))
		cmd->pipefd[1] = pipes[i][1];
}

void	close_unused_pipes(t_cmd_data *cmd, int n_cmds, int (*pipes)[2])
{
	int	j;

	j = 0;
	if (!pipes)
		return ;
	while (j < n_cmds - 1)
	{
		if (cmd->pipefd[0] != pipes[j][0])
			close(pipes[j][0]);
		if (cmd->pipefd[1] != pipes[j][1])
			close(pipes[j][1]);
		j++;
	}
}

void	perform_exec(t_node *node, t_program *prog, t_cmd_data *cmd)
{
	int	exit_status;

	if (cmd->redir && process_redir(cmd, prog) != 0)
	{
		close_fd(&prog->fd_in_orig);
		close_fd(&prog->fd_out_orig);
		exit(1);
	}
	set_final_fds(cmd);
	if (is_builtin(node->u_data.cmd.argv[0]))
	{
		exit_status = execute_builtin(prog, node, true);
		close_fd(&prog->fd_in_orig);
		close_fd(&prog->fd_out_orig);
		exit(exit_status);
	}
	close_fd(&prog->fd_in_orig);
	close_fd(&prog->fd_out_orig);
	exec_cmd_inchild(node);
	exit(EXIT_FAILURE);
}

void	free_resources(t_node **cmds, int (*pipes)[2], pid_t *pids)
{
	if (pids)
		free(pids);
	if (pipes)
		free(pipes);
	if (cmds)
		free(cmds);
}

int	copy_word(char **argv, t_token *tok, int i)
{
	argv[i] = ft_strdup(tok->txt);
	if (!argv[i])
	{
		free_partial_arr(argv, i);
		return (-1);
	}
	return (0);
}
