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

/*
 *	@brief Prints all currently exported environment variables
 *	in the declare -x format
 *	@returns 0 on success and 1 on error
 */


char **dup_envp(char **envp)
{
	int   n;
	int   i;
	char **copy;

	n = env_count(envp);
	copy = (char **)malloc(sizeof(char *) * (n + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < n)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			while (i > 0)
			{
				i--;
				free(copy[i]);
			}
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[n] = NULL;
	return (copy);
}

void sort_envp(char **envp)
{
	int   i;
	int   j;
	int   n;
	char *tmp;

	n = env_count(envp);
	i = 0;
	while (i < n)
	{
		j = i + 1;
		while (j < n)
		{
			if (ft_strcmp(envp[i], envp[j]) > 0)
			{
				tmp = envp[i];
				envp[i] = envp[j];
				envp[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void print_decl_line(char *entry)
{
	char *eq;

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

int export_print_all(char **envp)
{
	char **copy;
	int    i;

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

int	my_export(t_program *program, t_node *node)
{
	char			**args;
	int				i;
	int				status;
	char			*key;
	char			*val;
	t_export_mode	mode;

	args = node->u_data.cmd.argv;
	status = 0;
	if (!args || !args[0])
		return (0);
	if (!args[1])
		return (export_print_all(program->envp_cpy));
	i = 1;
	while (args[i])
	{
		key = NULL;
		val = NULL;
		if (parse_export_arg(args[i], &key, &val, &mode) != 0 || !is_identifier_good(key))
		{
			export_error_identifier(args[i]);
			status = 1;
		}
		else
		{
			if (mode == EXP_SET)
				env_set(&program->envp_cpy, key, val);
			else if (mode == EXP_APPEND)
				env_append(&program->envp_cpy, key, val);
			else
				env_mark_export(&program->envp_cpy, key);
		}
		if (key)
			free(key);
		if (val)
			free(val);
		i++;
	}
	return (status);
}
// int	my_export(t_program *program, t_node *node)
// {
// 	int		i;
// 	char	**args;
// 	int		nb_args;
// 	char	*equals_sign;
//
// 	fprintf(stderr, MAGENTA BOLD "MY EXPORT is about to be run\n" RESET);
// 	args = node->u_data.cmd.argv;
// 	nb_args = 0;
// 	while (args && args[nb_args])
// 		nb_args++;
// 	if (nb_args == 1)
// 	{
// 		if (!program->envp_cpy)
// 		{
// 			fprintf(stderr, BLUE "export: no environment variable found\n" RESET);
// 			return (1);
// 		}
// 		i = 0;
// 		while (program->envp_cpy[i])
// 		{
// 			equals_sign = ft_strchr(program->envp_cpy[i], '=');;
// 			if (equals_sign)
// 			{
// 				*equals_sign = '\0';
// 				printf("declare -x %.*s=\"%s\"\n", (int)(equals_sign - program->envp_cpy[i]), program->envp_cpy[i], equals_sign + 1);
// 				*equals_sign = '=';
// 			}
// 			else
// 				printf("declare -x %s\n", program->envp_cpy[i]);
// 			i++;
// 		}
// 	}
// 	return (0);
// }

