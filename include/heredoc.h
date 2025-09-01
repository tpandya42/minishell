#ifndef HEREDOC_H
#define HEREDOC_H

# include "minishell.h"
int is_quoted(const char *s);
char *strip_outer_quotes(const char *s);
int	heredoc_prepare(t_redir *redir, char **envp_cpy, int last_exit);
void	heredoc_normalize_delimiter(t_redir *redir);

#endif
