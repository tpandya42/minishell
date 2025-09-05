#include "minishell.h"


sig_atomic_t	g_signal_value = 0;

// ---  CTRL + C at PROMPT --- //
void sigint_prompt(int signum)
{
	(void)signum;
	g_signal_value = SIGINT;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

// --- CTRL + C DURING HEREDOC --- //
// void sigint_heredoc(int signum)
// {
// 	(void)signum;
// 	g_signal_value = SIGINT;
// 	write(STDOUT_FILENO, "\n", 1);  // Write a newline for visual feedback
	
// 	// Force readline to return immediately
// 	rl_done = 1;
// }

void set_signal_handler(int signum, void(*handler)(int))
{
	struct sigaction sa;
	sa.sa_handler = handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(signum, &sa, NULL);
}

// --- CONTEXT-SPECIFIC SIGNAL SETUP --- //
void set_signal_prompt(void)
{
	set_signal_handler(SIGINT, sigint_prompt);
	set_signal_handler(SIGQUIT, SIG_IGN);
}

void set_signal_child_process(void)
{
	set_signal_handler(SIGINT, SIG_DFL);
	set_signal_handler(SIGQUIT, SIG_DFL);
}

void set_signal_heredoc(void)
{
	// set_signal_handler(SIGINT, sigint_heredoc);
	set_signal_handler(SIGINT, SIG_DFL); //new
	set_signal_handler(SIGQUIT, SIG_IGN);
}

void set_signal_main_process(void)
{
	set_signal_handler(SIGINT, SIG_IGN);
	set_signal_handler(SIGQUIT, SIG_IGN);
}

