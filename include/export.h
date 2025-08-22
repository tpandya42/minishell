#ifndef EXPORT_H
#define EXPORT_H

#include "minishell.h"

int	is_identifier_good(const char *s);
int key_len_in_entry(const char *entry);
int	env_count(char **envp);
int env_find_index(char **envp, const char *key);
char	*make_kv_string(const char *key, const char *value);
const char *entry_value_ptr(const char *entry);
int env_set(char ***penvp, const char *key, const char *value);
int env_append(char ***penvp, const char *key, const char *suffix);
int env_mark_export(char ***penvp, const char *key);
int parse_export_arg(const char *arg, char **key_out, char **val_out, t_export_mode *mode_out);
void    export_error_identifier(const char *arg);
char **dup_envp(char **envp);
void sort_envp(char **envp);
void print_decl_line(char *entry);
int export_print_all(char **envp);
int	my_export(t_program *program, t_node *node);

#endif

