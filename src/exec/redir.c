/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:59:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/29 14:33:29 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_in(int fd)
{
	if (fd >= 0)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("dup2 redir_in");
			return (1);
		}
	}
	return (0);
}

int	redir_out(int fd)
{
	if (fd >= 0)
	{
		// Flush stdout before redirecting
		fflush(stdout);
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 redir_out");
			return (1);
		}
	}
	return (0);
}

int	setup_redir(t_cmd_data *cmd)
{
	if (cmd->fd_in != STDIN_FILENO && cmd->fd_in >= 0
		&& cmd->fd_in != cmd->pipefd[0])
	{
		// fprintf(stderr, "[DEBUG] setup_redir: dup2(%d -> STDIN_FILENO)\n", cmd->fd_in);//test
		DEBUG_PRINT("[DEBUG] setup_redir: dup2(%d -> STDIN_FILENO)\n", cmd->fd_in);//debug
		fprintf(stderr, "DEBUG: Redirecting stdin from fd=%d\n", cmd->fd_in);
		if (redir_in(cmd->fd_in) != 0)
			return (1);
		// Don't close the input fd yet - keep it for the command to use
		// close_fd(&cmd->fd_in);
	}
	if (cmd->fd_out != STDOUT_FILENO && cmd->fd_out >= 0
		&& cmd->fd_out != cmd->pipefd[1])
	{
		// fprintf(stderr, "[DEBUG] setup_redir: dup2(%d -> STDOUT_FILENO)\n", cmd->fd_out);//test
		DEBUG_PRINT("[DEBUG] setup_redir: dup2(%d -> STDOUT_FILENO)\n", cmd->fd_out);//test
		fprintf(stderr, "DEBUG: Redirecting stdout to fd=%d\n", cmd->fd_out);
		if (redir_out(cmd->fd_out) != 0)
			return (1);
		// Don't close the output fd yet - keep it for the command to use
		// close_fd(&cmd->fd_out);
	}
	// fprintf(stderr, GREEN "Redirections set up successfully.\n" RESET); //test
	DEBUG_PRINT(GREEN "Redirections set up successfully.\n" RESET); //test
	return (0);
}


static void	update_redir_fd(int new_fd, int *cmd_fd)
{
	if (new_fd == -1)
		return ;
	if (*cmd_fd != STDIN_FILENO && *cmd_fd != STDOUT_FILENO)
		close_fd(cmd_fd);
	*cmd_fd = new_fd;
}


int	process_redir(t_cmd_data *cmd, t_program *program)
{
	t_redir	*r;

	r = cmd->redir;
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	while (r)
	{
		if (r->type == RED_HERE_DOC)
		{
			if (heredoc_prepare(r, program->envp_cpy, program->last_exit_status) != 0)
				return (1);
            // only dup later in child
            update_redir_fd(r->fd, &cmd->fd_in);
        }
        else
        {
            // fprintf(stderr, BLUE "Attempting to open file: %s\n" RESET, r->target);//test
            DEBUG_PRINT(BLUE "Attempting to open file: %s\n" RESET, r->target);//test
            if (open_redir_filename(r) != 0)
                return 1;
            // fprintf(stderr, GREEN "Successfully opened fd %d for %s\n" RESET, r->fd, r->target);//debug
            DEBUG_PRINT(GREEN "Successfully opened fd %d for %s\n" RESET, r->fd, r->target);//debug
            if (r->type == RED_IN)
                update_redir_fd(r->fd, &cmd->fd_in);
            else
                update_redir_fd(r->fd, &cmd->fd_out);
        }
        r = r->next;
    }
    return 0;
}

