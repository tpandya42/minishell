# Signal Handling in Milanshell

This document explains all functions implemented in `signal.c` for handling signals in the shell.  
These functions allow Milanshell to behave similarly to Bash regarding Ctrl+C, Ctrl+D, and Ctrl+\.

---

## Global Variable

```c
sig_atomic_t g_signal_value;
```

Purpose: Safely tracks signals across the shell.

Type: sig_atomic_t ensures atomic read/write from signal handlers.

Usage: Updated inside signal handlers (e.g., SIGINT) to inform shell logic.



## sigint_prompt
void sigint_prompt(int signum);
Purpose: Handles SIGINT (Ctrl+C) when the shell is at the interactive prompt.

Parameters:

signum — signal number (typically SIGINT).

Behavior:

Clears the current input line.

Moves cursor to a new line.

Redisplays the shell prompt.

Sets g_signal_value to SIGINT.

Usage: Called by the signal handler for SIGINT in interactive mode.



## sigint_heredoc
void sigint_heredoc(int signum);
Purpose: Handles SIGINT (Ctrl+C) during heredoc input.

Parameters:

signum — signal number (typically SIGINT).

Behavior:

Sets g_signal_value to SIGINT.

Sends a newline to the input buffer to terminate heredoc.

Temporarily modifies terminal settings (ICANON and ECHO) to safely inject input.

Restores terminal settings afterward.

Usage: Called by the signal handler during heredoc reading.


## set_signal_handler
void set_signal_handler(int signum, void (*handler)(int));
Purpose: Generic function to assign a signal handler to a signal.

Parameters:

signum — signal number (e.g., SIGINT, SIGQUIT).

handler — function pointer to the handler, or SIG_IGN / SIG_DFL.

Behavior:

Configures a sigaction structure.

Sets the signal handler for the given signal.

Clears signal mask during handler execution.

Usage: Helper function for configuring signals in different shell states.


## set_signal_child_process
void set_signal_child_process(void);
Purpose: Configures signal handling for forked child processes.

Behavior:

SIGINT → default (terminates child process).

SIGQUIT → default (terminates child process).

Usage: Call immediately after fork() in the child process before executing a command


## set_signal_heredoc
void set_signal_heredoc(void);
Purpose: Configures signal handling while reading heredoc input.

Behavior:

SIGINT → sigint_heredoc.

SIGQUIT → ignored (Ctrl+\ does nothing).

Usage: Call before reading heredoc input to ensure proper interruption handling.

## set_signal_main_process
void set_signal_main_process(void);
Purpose: Configures signal handling for the main shell process (optional).

Behavior:

SIGINT → ignored (shell does not terminate on Ctrl+C).

SIGQUIT → ignored.

Usage: Optional, can be used when main shell loop should temporarily ignore signals.





# SIGNAL HANDLING DIAGRAM
[Interactive Prompt] ----SIGINT---> sigint_prompt
                     ----SIGQUIT---> ignored

[Heredoc Read] ------SIGINT---> sigint_heredoc
                     ----SIGQUIT---> ignored

[Child Process] -----SIGINT---> default
                     ----SIGQUIT---> default

[Main Shell] -------SIGINT---> ignored
                     ----SIGQUIT---> ignored

