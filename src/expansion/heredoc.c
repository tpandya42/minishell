/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 17:29:36 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/08 08:28:28 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Close read end in child
// Set signal handlers for heredoc
// Reset signal value
// Read lines until delimiter or signal
// if (!line) is EOF detected
//check delimmiter
//process line heredoc
static void	child_heredoc(t_redir *redir, t_program *program, int pipefd[2])
{
	t_heredoc	hd;

	init_hd(&hd, redir, program, pipefd);
	close_fd(&hd.pipefd[0]);
	set_signal_heredoc();
	g_signal_value = 0;
	heredoc_normalize_delimiter(redir);
	hd.current_line = read_heredoc(&hd);
	if (redir->hd_expand && hd.buf)
		expand_heredoc(program, &hd.buf);
	if (hd.buf)
		write(pipefd[1], hd.buf, ft_strlen(hd.buf));
	free_hd(&hd);
	close_fd(&pipefd[1]);
	exit(0);
}

//parent after ctrl c
static int	clean_ctrl_c_heredoc(t_program *program, int *pipefd)
{
	g_signal_value = SIGINT;
	close_fd(&pipefd[0]);
	if (program->line)
	{
		free(program->line);
		program->line = NULL;
	}
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay(); 
	return (1);
}

static int	other_error_heredoc(int *pipefd)
{
	close_fd(&pipefd[0]);
	return (1);
}

// Close write end in parent
// Store read end for command
// Wait for child to finish
// If child was interrupted by Ctrl+C (status 130)
//handles other errors
//restore terminal when success and set signal promopt
static int	parent_heredoc(t_redir *redir,
		t_program *program, int pipefd[2], int pid)
{
	int	status;

	status = 0;
	close_fd(&pipefd[1]);
	redir->fd = pipefd[0];
	set_signal_prompt(1);
	waitpid(pid, &status, 0);
	set_signal_prompt(0);
	if ((WIFEXITED(status) && WEXITSTATUS(status) == 130)
		|| (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT))
		clean_ctrl_c_heredoc(program, &pipefd[0]);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		other_error_heredoc(&pipefd[0]);
	tcsetattr(STDIN_FILENO, TCSANOW, &program->orig_termios);
	set_signal_prompt(0);
	return (0);
}

/* prepare heredoc: create a pipe, read user input until delimiter */
// int heredoc_prepare(t_redir *redir, char **envp, int last_exit)
// Fork a child process for the heredoc
int	heredoc_prepare(t_redir *redir, t_program *program)
{
	int		pipefd[2];
	pid_t	pid;

	if (!redir || pipe(pipefd) == -1)
	{
		perror("heredoc: pipe failed");
		return (1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("heredoc: fork failed");
		close_fd(&pipefd[0]);
		close_fd(&pipefd[1]);
		return (1);
	}
	if (pid == 0)
		child_heredoc(redir, program, pipefd);
	return (parent_heredoc (redir, program, pipefd, pid));
}
