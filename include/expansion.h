#ifndef EXPANSION_H
# define EXPANSION_H
# include "minishell.h"

void	expand(t_token *token, char **envp_cpy, int last_exit);
char	*expand_token_text(char *txt, char **envp_cpy, int last_exit);


#endif
