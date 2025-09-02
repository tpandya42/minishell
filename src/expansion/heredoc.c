
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


    if (!redir || pipe(pipefd) == -1)
    {
        perror("heredoc: pipe failed");
        return 1;
    }

    // Always normalize delimiter before heredoc input
    heredoc_normalize_delimiter(redir);
    redir->fd = pipefd[0]; // read end for child/STDIN

    set_signal_heredoc(); // handle Ctrl-C in heredoc

    while (1)
    {
        line = readline("> ");
        if (!line) // EOF (Ctrl-D)
            break;

        if (is_delim_line(line, redir->target))
        {
            free(line);
            break;
        }

        char *to_write;
        if (redir->hd_expand)
        {
            to_write = expand_token_text(line, envp, last_exit);
        }
        else
        {
            to_write = ft_strdup(line);
        }

        free(line);

        if (!to_write)
        {
            close(pipefd[0]);
            close(pipefd[1]);
            set_signal_prompt();
            return 1;
        }

        write(pipefd[1], to_write, ft_strlen(to_write));
        write(pipefd[1], "\n", 1); // preserve newline for heredoc
        free(to_write);
    }

    close(pipefd[1]); // close write end
    set_signal_prompt();
    return 0;
}

