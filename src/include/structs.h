/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 09:56:45 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/05 08:18:08 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

// --- FORWARD DECLARACTIONS ---//
typedef struct s_redir		t_redir;

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
}							t_toktype;

// --- EXPORT MODE ENUM --- //
typedef enum e_export_mode
{
	EXP_MARK = 0,
	EXP_SET = 1,
	EXP_APPEND = 2
}							t_export_mode;

// --- COMMAND TYPE --//

typedef enum e_cmdtype
{
	EXECUTABLE,
	BUILTIN,
}							t_cmdtype;

// --- NODE TYPE --- //

typedef enum e_nodetype
{
	OPERATOR,
	COMMAND
}							t_nodetype;

// --- EXECUTION REDIR --- //

typedef enum e_redir_type
{
	RED_IN,
	RED_OUT,
	RED_APPEND,
	RED_HERE_DOC,
	RED_UNKNOW,
	RED_QUOTED
}							t_redir_type;

// -----------------------------------------//
//                AST STRUCTS               //
// -----------------------------------------//

typedef struct s_pipe_ctx
{
	int						idx;
	int						n_cmds;
	int						(*pipes)[2];
}							t_pipe_ctx;

// --- TOKEN STRUCT --- //

typedef struct s_token
{
	char					*txt;
	enum e_toktype			type;
	struct s_token			*next;
	char					delim;
}							t_token;

// --- COMMAND NODE --- //

typedef struct s_cmd_data
{
	t_token					*tokens;
	char					**argv;
	char					**env;
	t_redir					*redir;
	t_cmdtype				cmd_type;
	int						fd_in;
	int						fd_out;
	int						pipefd[2];
}							t_cmd_data;

// --- OPERATOR DATA ---//

typedef struct s_operator
{
	enum e_toktype			type;
	struct s_node			*left;
	struct s_node			*right;
}							t_operator;

// --- UNION FOR NODE CONTENT --- //

union						u_node_content
{
	t_operator				op;
	t_cmd_data				cmd;
};

// --- STRUCT FOR ALL NODES --- //
typedef struct s_node
{
	enum e_nodetype			type;
	union u_node_content	u_data;
}							t_node;

typedef struct s_program
{
	char					*line;
	char					**envp;
	char					**envp_cpy;
	t_node					*root;
	t_token					*token_list;
	int						fd_in_orig;
	int						fd_out_orig;
	int						last_exit_status;
	struct termios			orig_termios;
}							t_program;

typedef struct s_spawn_ctx
{
	t_node					**cmds;
	int						n_cmds;
	int						(*pipes)[2];
	pid_t					*pids;
	t_program				*prog;
}							t_spawn_ctx;

// --- STRUCT FOR REDIR --- //

typedef struct s_redir
{
	enum e_redir_type		type;
	char					*target;
	int						fd;
	struct s_redir			*next;
	bool					hd_expand;
	bool					quoted;
}							t_redir;

#endif
