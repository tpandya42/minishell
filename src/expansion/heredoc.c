
#include "minishell.h"


/* ---- local helpers ----------------------------------------------------- */

int is_quoted(const char *s)
{
    if (!s) return 0;
    size_t len = ft_strlen(s);
    return (len >= 2 && ((s[0] == '\'' && s[len-1] == '\'') ||
                         (s[0] == '"'  && s[len-1] == '"')));
}

char *strip_outer_quotes(const char *s)
{
    if (!s) return NULL;
    size_t len = ft_strlen(s);
    if (len >= 2 && ((s[0] == '\'' && s[len-1] == '\'') ||
                     (s[0] == '"'  && s[len-1] == '"')))
        return ft_substr(s, 1, len - 2);
    return ft_strdup(s);
}

static int is_delim_line(const char *line, const char *delim)
{
    if (!line || !delim) return 0;
    return (ft_strcmp(line, delim) == 0);
}

/* ---- public helpers ---------------------------------------------------- */

/* normalize the delimiter: strip quotes and set expand flag */
void heredoc_normalize_delimiter(t_redir *redir)
{
    if (!redir || !redir->target) return;

    // Use quoted field from redir struct
    redir->hd_expand = !redir->quoted;

    char *clean = strip_outer_quotes(redir->target);
    if (clean)
    {
        free(redir->target);
        redir->target = clean;
    }
}

/* prepare heredoc: create a pipe, read user input until delimiter */
int heredoc_prepare(t_redir *redir, char **envp, int last_exit)
{
    int pipefd[2];
    char *line;
    pid_t pid;
    int status;
    int current_line;//new for warning message

    current_line = 1;//new to count lines in here doc for warning message
    if (!redir || pipe(pipefd) == -1)
    {
        perror("heredoc: pipe failed");
        return 1;
    }
    // Fork a child process for the heredoc
    pid = fork();
    if (pid < 0)
    {
        perror("heredoc: fork failed");
        close_fd(&pipefd[0]);
        close_fd(&pipefd[1]);
        return 1;
    }
    
    if (pid == 0)
    {
        // Child process
        close_fd(&pipefd[0]);   // Close read end in child
        set_signal_heredoc();   // Set signal handlers for heredoc
        g_signal_value = 0;     // Reset signal value
        heredoc_normalize_delimiter(redir);//new position
        while (1)               // Read lines until delimiter or signal
        {
            line = readline("> ");
            
            // Check for EOF or signal
            // if (!line || g_signal_value == SIGINT)
            if (!line)//changed
            {
                fprintf(stderr, "warning: here-document at line %d delimited by end-of-file (wanted `%s')\n",
                        current_line,
                        redir->target);  
                break;//new
                // if (line)
                    // free(line);
                // close(pipefd[1]);
                
                // Exit with 130 if interrupted by Ctrl+C, or 0 for EOF
                // exit(g_signal_value == SIGINT ? 130 : 0);//ternary not allowed
            }
            
            // Check for delimiter
            if (is_delim_line(line, redir->target))
            {
                free(line);
                break;
            }
            
            // Process the line
            char *to_write;
            if (redir->hd_expand)
            {
                // DEBUG removed
                to_write = expand_token_text(line, envp, last_exit);
            }
            else
            {
                // DEBUG removed
                to_write = ft_strdup(line);
            }
            
            free(line);
            
            if (!to_write)
            {
                close_fd(&pipefd[1]);
                exit(1);
            }
            
            // Write to pipe
            write(pipefd[1], to_write, ft_strlen(to_write));
            write(pipefd[1], "\n", 1);  // Preserve newline
            free(to_write);
            current_line++;//new for warning message
        }
        
        // Clean up and exit
        close_fd(&pipefd[1]);
        exit(0);
    }
    
    // Parent process
    close_fd(&pipefd[1]);  // Close write end in parent
    
    // Store read end for command
    redir->fd = pipefd[0];
    
    // Wait for child to finish
    waitpid(pid, &status, 0);
    
    // If child was interrupted by Ctrl+C (status 130)
    if (WIFEXITED(status) && WEXITSTATUS(status) == 130)//check if better == SIGINT instead of 130
    {
        g_signal_value = SIGINT;
        close_fd(&pipefd[0]);
        set_signal_prompt();
        return 1;
    }
    
    // Handle other errors
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
    {
        close_fd(&pipefd[0]);
        set_signal_prompt();
        return 1;
    }
    
    // Success
    set_signal_prompt();
    return 0;
}
