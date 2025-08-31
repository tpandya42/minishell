/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 09:56:45 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/23 07:30:20 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

// --- FORWARD DECLARACTIONS ---//
typedef struct s_redir	t_redir;

// -----------------------------------------//
//            GLOBAL ENUMS                  //
// -----------------------------------------//

// --- TOKEN TYPES --- //

typedef enum e_toktype
{
	WORD = 1,
	HEREDOC,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	OPEN,
	CLOSE,
	PIPE,
	AND,
	OR,
	SEMICOLON,
	MAX_TYPE,
	NO_Q,
	SINGLE_Q,
	DOUBLE_Q
}	t_toktype;

// --- EXPORT MODE ENUM --- //
typedef enum e_export_mode
{
	EXP_MARK = 0,
	EXP_SET = 1,
	EXP_APPEND = 2
} t_export_mode;


// --- COMMAND TYPE --//

typedef enum e_cmdtype
{
	EXECUTABLE,
	BUILTIN,
}	t_cmdtype;

// --- NODE TYPE --- //

typedef enum e_nodetype
{
	OPERATOR,
	COMMAND
	// MAX_NODETYPE
}t_nodetype;

// --- EXECUTION REDIR --- //

typedef enum e_redir_type
{
	RED_IN,
	RED_OUT,
	RED_APPEND,
	RED_HERE_DOC,
	RED_UNKNOW
}t_redir_type;

// -----------------------------------------//
//                AST STRUCTS               //
// -----------------------------------------//

// --- TOKEN STRUCT --- //

typedef struct s_token
{
	char			*txt;
	enum e_toktype	type;
	struct s_token	*next;
	char			delim;
}	t_token;


// --- COMMAND NODE --- //

typedef struct s_cmd_data
{
	t_token		*tokens;
	char		**argv;
	char		**env;
	t_redir		*redir;//redir struct in cmd_node
	t_cmdtype	cmd_type;
	int			fd_in;
	int			fd_out;
}	t_cmd_data;

// --- OPERATOR DATA ---//

typedef struct s_operator
{
	enum e_toktype	type;
	struct s_node	*left;
	struct s_node	*right;
}	t_operator;

// --- UNION FOR NODE CONTENT --- //

union u_node_content
{
	t_operator	op;
	t_cmd_data	cmd;
};

// --- STRUCT FOR ALL NODES --- //
typedef struct s_node 
{
	enum e_nodetype			type;
	union u_node_content	u_data;
}	t_node;

typedef struct s_program
{
	char	*line;
	char	**envp;
	char	**envp_cpy;
	t_node	*root;
	t_token	*token_list;
	int		fd_in_orig;//at the end of redir
	int		fd_out_orig;//at the end of redir
	int		last_exit_status;
}t_program;

// --- STRUCT FOR REDIR --- //

typedef struct s_redir
{
	enum e_redir_type	type;
	char				*target;//filename for < > >> and delimiter for <<
	int					fd;//associated with the redir
	struct s_redir		*next;//linkedlist
}	t_redir;


#endif
