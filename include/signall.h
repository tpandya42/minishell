#ifndef SIGNALL_H
#define SIGNALL_H	

#include "minishell.h"

extern sig_atomic_t g_signal_value;
void	sigint_prompt(int signum);

void	sigint_heredoc(int signum);

void	set_signal_handler(int signum, void(*handler)(int));

void set_signal_prompt(void);

void set_signal_child_process(void);

void set_signal_heredoc(void);

#endif
