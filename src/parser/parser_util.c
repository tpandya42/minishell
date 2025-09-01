/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 08:22:30 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/29 14:21:15 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	init_cmd_node(t_token *token, t_node *node)
{
	node->type = COMMAND;
	node->u_data.cmd.tokens = token;
	node->u_data.cmd.redir = NULL;
	node->u_data.cmd.argv = NULL;
	node->u_data.cmd.cmd_type = EXECUTABLE;
	node->u_data.cmd.fd_in = STDIN_FILENO;
	node->u_data.cmd.fd_out = STDOUT_FILENO;
	node->u_data.cmd.pipefd[0] = -1;
	node->u_data.cmd.pipefd[1] = -1;
}

t_token	*token_cpy(t_token *token)
{
	t_token	*new_token;

	new_token = malloc (sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->txt = ft_strdup(token->txt);
	if (!new_token->txt)
	{
		free (new_token);//check if smt else
		return (NULL);
	}
	new_token->type = token->type;
	new_token->next = NULL;//check where to free new_token
	return (new_token);
}

void	add_token(t_token **list, t_token *new_token)
{
	t_token	*tmp;

	if (!new_token)
		return ;
	if (!*list)
	{
		*list = new_token;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp ->next;
	tmp->next = new_token;
}

void	error_split_arg(t_token *current)
{
	if (current->next)
		fprintf(stderr, BOLD RED
			"syntax error near unexpected token '%s'\n" RESET,
			current->next->txt);
	else
		fprintf(stderr, BOLD RED
			"syntax error near unexpected token 'newline'\n" RESET);
}

int	split_cmd_arg(t_token **current, t_cmd_data *cmd_data)
{
	// fprintf(stderr, "\033[1;36m[DEBUG] split_cmd_arg: called on token '%s' type %d\033[0m\n", (*current)->txt, (*current)->type);
	DEBUG_PRINT("\033[1;36m[DEBUG] split_cmd_arg: called on token '%s' type %d\033[0m\n", (*current)->txt, (*current)->type);//debug
	if (!(*current)->next ||
		((*current)->next->type != WORD &&
		 (*current)->next->type != SINGLE_Q &&
		 (*current)->next->type != DOUBLE_Q))
	{
		error_split_arg(*current);
		fprintf(stderr, BOLD RED "Error: Redirection target missing or invalid.\n" RESET);//debug
		return (1);
	}
	bool quoted = ((*current)->next->type == SINGLE_Q || (*current)->next->type == DOUBLE_Q);
	add_redir(&cmd_data->redir,
		create_redir_node((*current)->next->txt, map_type((*current)->type), quoted));
	// Count redirs after adding
	int redir_count = 0;
	t_redir *tmp_redir = cmd_data->redir;//make happy norm
	while (tmp_redir) { redir_count++; tmp_redir = tmp_redir->next; }
	// fprintf(stderr, "\033[1;36m[DEBUG] split_cmd_arg: cmd_data->redir now has %d redirs\033[0m\n", redir_count);//DEBUG
	DEBUG_PRINT("\033[1;36m[DEBUG] split_cmd_arg: cmd_data->redir now has %d redirs\033[0m\n", redir_count);//DEBUG
	*current = (*current)->next->next;
	return (0);
}
char	**copy_token_strings(t_token *cmd_tokens, char **argv)
{
	t_token	*current;
	int		i;

	i = 0;
	current = cmd_tokens;
	while (current)
	{
		argv[i] = ft_strdup(current->txt);
		if (!argv[i])
		{
			free_array(argv);
			perror("Failed ft_strdup for argv");
			return (NULL);
		}
		i++;
		current = current->next;
	}
	argv[i] = NULL;
	return (argv);
}

int	ft_lstsize(t_token *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

char	**build_argv_from_tokens(t_token *cmd_tokens)
{
	char	**argv;

	argv = malloc(sizeof(char *) * (ft_lstsize(cmd_tokens) + 1));
	if (!argv)
	{
		free_token(cmd_tokens);
		perror("Failed to allocate argv");
		return (NULL);
	}
	argv = copy_token_strings(cmd_tokens, argv); 
	if (!argv)
	{
		free_token(cmd_tokens);
		return (NULL);
	}
	return (argv);
}


int	process_cmd_tokens(t_token *token, t_cmd_data *cmd_data)
{
    t_token *current;
    t_token *cmd_tokens;
    // Print enum values for debugging
    // fprintf(stderr, "\033[1;36m[DEBUG] REDIR_IN=%d REDIR_OUT=%d APPEND=%d HEREDOC=%d\033[0m\n", REDIR_IN, REDIR_OUT, APPEND, HEREDOC);
    DEBUG_PRINT("\033[1;36m[DEBUG] REDIR_IN=%d REDIR_OUT=%d APPEND=%d HEREDOC=%d\033[0m\n", REDIR_IN, REDIR_OUT, APPEND, HEREDOC);//debug
    // Debug print: list all tokens and their types
    // fprintf(stderr, "\033[1;36m[DEBUG] process_cmd_tokens: tokens received:\033[0m\n");
    DEBUG_PRINT("\033[1;36m[DEBUG] process_cmd_tokens: tokens received:\033[0m\n");//debug
    current = token;
    while (current) {
        // fprintf(stderr, "\033[1;36m  token: '%s' type: %d\033[0m\n", current->txt, current->type);
        DEBUG_PRINT("\033[1;36m  token: '%s' type: %d\033[0m\n", current->txt, current->type);//DEBUG
        current = current->next;
    }
    // Reset current for actual processing
    current = token;
    cmd_tokens = NULL;
    while (current)
    {
        if (current->type == REDIR_IN || current->type == REDIR_OUT ||
            current->type == APPEND || current->type == HEREDOC)
        {
            // fprintf(stderr, YELLOW "Found redirection token: '%s'. Splitting...\n" RESET, current->txt);//debug
            DEBUG_PRINT(YELLOW "Found redirection token: '%s'. Splitting...\n" RESET, current->txt);//debug
            if (split_cmd_arg(&current, cmd_data) != 0)
            {
                free_token(cmd_tokens);
                return (1);
            }
            // split_cmd_arg already advances current
            continue;
        }
        // fprintf(stderr, GREEN "Adding command token: '%s'\n" RESET, current->txt);//debug
        DEBUG_PRINT(stderr, GREEN "Adding command token: '%s'\n" RESET, current->txt);//debug
        add_token(&cmd_tokens, token_cpy(current));
        current = current->next;
    }
    cmd_data->tokens = cmd_tokens;
    cmd_data->argv = build_argv_from_tokens(cmd_tokens);
    if (!cmd_data->argv)
    {
        perror("Failed to allocate argv");
        return (1);
    }
    return (0);
}
