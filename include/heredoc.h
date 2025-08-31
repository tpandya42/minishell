#ifndef HEREDOC_H
#define HEREDOC_H

#include "minishell.h"
int process_heredoc(t_redir *redir, t_program *program);

int	handle_heredocs(t_cmd_data *cmd, t_program *program);
void free_heredocs(t_redir *redir);
#endif
