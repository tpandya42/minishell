/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rococo <rococo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 11:12:31 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/15 15:20:08 by rococo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_decl_line(char *entry)
{
	char	*eq;

	eq = ft_strchr(entry, '=');
	if (eq)
	{
		*eq = '\0';
		printf("declare -x %s=\"%s\"\n", entry, eq + 1);
		*eq = '=';
	}
	else
		printf("declare -x %s\n", entry);
}

int	export_print_all(char **envp)
{
	char	**copy;
	int		i;

	copy = dup_envp(envp);
	if (!copy)
		return (1);
	sort_envp(copy);
	i = 0;
	while (copy[i])
	{
		print_decl_line(copy[i]);
		free(copy[i]);
		i++;
	}
	free(copy);
	return (0);
}

static int	handle_export_action(t_program *program, char *key, char *val,
		t_export_mode mode)
{
	if (mode == EXP_SET)
		env_set(&program->envp_cpy, key, val);
	else if (mode == EXP_APPEND)
		env_append(&program->envp_cpy, key, val);
	else
		env_mark_export(&program->envp_cpy, key);
	return (0);
}

static int	process_single_arg(t_program *program, char *arg)
{
	char			*key;
	char			*val;
	t_export_mode	mode;

	key = NULL;
	val = NULL;
	if (parse_export_arg(arg, &key, &val, &mode) != 0
		|| !is_identifier_good(key))
	{
		export_error_identifier(arg);
		free(key);
		free(val);
		return (1);
	}
	handle_export_action(program, key, val, mode);
	free(key);
	free(val);
	return (0);
}

int	my_export(t_program *program, t_node *node)
{
	char	**args;
	int		i;
	int		status;

	args = node->u_data.cmd.argv;
	status = 0;
	if (!args || !args[0])
		return (0);
	if (!args[1])
		return (export_print_all(program->envp_cpy));
	i = 1;
	while (args[i])
	{
		if (process_single_arg(program, args[i]))
			status = 1;
		i++;
	}
	return (status);
}
