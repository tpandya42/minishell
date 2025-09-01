/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:54:38 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/26 08:58:46 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"


typedef enum e_builtin_type
{
	BUILTIN_NONE = 0,//default if is not
	BUILTIN_ECHO,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT,
}	t_builtin_type;

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

int		is_builtin(const char *cmd_name);
int		execute_builtin(t_program *program, t_node *node, bool is_pipe_child);
char	*find_env_value(char **envp, const char *key);
void	update_env_var(t_program *program, const char *key, const char *value);

// --- SPECIFICS --- //
int		my_echo(t_program *program, t_node *node);
int		my_pwd(t_program *program, t_node *node);
int		my_env(t_program *program, t_node *node);
void	update_env_var(t_program *program, const char *key, const char *value);
int		my_export(t_program *program, t_node *node);
void	my_exit(t_program *program, t_node *node);
int		my_cd(t_program *program, t_node *node);
void	update_free_paths(t_program *program,
			char *old_pwd, char *new_cwd, char *dest_path);
int		handle_env_path(t_program *program, char *key, char **dest_path);
int		handle_cwd_error(char *dest_path);

#endif