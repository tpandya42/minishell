/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:29:12 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/10 14:29:13 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT_H
# define EXPORT_H

# include "minishell.h"

int			is_identifier_good(const char *s);
int			key_len_in_entry(const char *entry);
int			env_count(char **envp);
int			env_find_index(char **envp, const char *key);
char		*make_kv_string(const char *key, const char *value);
const char	*entry_value_ptr(const char *entry);
int			env_set(char ***penvp, const char *key, const char *value);
int			env_append(char ***penvp, const char *key, const char *suffix);
int			env_mark_export(char ***penvp, const char *key);
int			parse_export_arg(const char *arg, char **key_out, char **val_out,
				t_export_mode *mode_out);
void		export_error_identifier(const char *arg);
char		**dup_envp(char **envp);
void		sort_envp(char **envp);
void		print_decl_line(char *entry);
int			export_print_all(char **envp);
int			my_export(t_program *program, t_node *node);
int			env_replace(char ***penvp, int idx, char *kv);
int			env_extend(char ***penvp, char *kv);
int			env_update_append(char ***penvp, int idx, const char *key,
				const char *suffix);
int			parse_append(const char *arg, int i, char **key_out,
				char **val_out);
int			parse_set(const char *arg, int i, char **key_out, char **val_out);
#endif
