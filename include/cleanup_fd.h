#ifndef CLEANUP_FD_H
#define CLEANUP_FD_H

#include "minishell.h"

// Function to clean up file descriptors after command execution
void cleanup_fds(t_cmd_data *cmd);

#endif
