#include "minishell.h"

int env_set(char ***penvp, const char *key, const char *value)
{
	int   idx;
	int   n;
	char *kv;
	char **new_env;
	int   i;

	if (!penvp || !key)
		return (-1);
	kv = make_kv_string(key, value);
	if (!kv)
		return (-1);
	idx = env_find_index(*penvp, key);
	if (idx >= 0)
	{
		free((*penvp)[idx]);
		(*penvp)[idx] = kv;
		return (0);
	}
	n = env_count(*penvp);
	new_env = (char **)malloc(sizeof(char *) * (n + 2));
	if (!new_env)
	{
		free(kv);
		return (-1);
	}
	i = 0;
	while (i < n)
	{
		new_env[i] = (*penvp)[i];
		i++;
	}
	new_env[n] = kv;
	new_env[n + 1] = NULL;
	free(*penvp);
	*penvp = new_env;
	return (0);
}


int env_append(char ***penvp, const char *key, const char *suffix)
{
	int   idx;
	char *joined;
	char *kv;
	const char *oldv;

	if (!penvp || !key || !suffix)
		return (-1);
	idx = env_find_index(*penvp, key);
	if (idx >= 0)
	{
		oldv = entry_value_ptr((*penvp)[idx]);
		joined = ft_strjoin((char *)oldv, (char *)suffix);
		if (!joined)
			return (-1);
		kv = make_kv_string(key, joined);
		free(joined);
		if (!kv)
			return (-1);
		free((*penvp)[idx]);
		(*penvp)[idx] = kv;
		return (0);
	}
	return (env_set(penvp, key, suffix));
}

int env_mark_export(char ***penvp, const char *key)
{
	if (!penvp || !key)
		return (-1);
	if (env_find_index(*penvp, key) >= 0)
		return (0);
	return (env_set(penvp, key, ""));
}


int parse_export_arg(const char *arg, char **key_out, char **val_out, t_export_mode *mode_out)
{
	int i;

	*key_out = NULL;
	*val_out = NULL;
	*mode_out = EXP_MARK;
	if (!arg || !*arg)
		return (-1);

	i = 0;
	while (arg[i] && arg[i] != '=' && arg[i] != '+')
		i++;

	if (arg[i] == '+' && arg[i + 1] == '=')
	{
		*mode_out = EXP_APPEND;
		*key_out = ft_substr(arg, 0, i);
		*val_out = ft_strdup(arg + i + 2);
	}
	else if (arg[i] == '=')
	{
		*mode_out = EXP_SET;
		*key_out = ft_substr(arg, 0, i);
		*val_out = ft_strdup(arg + i + 1);
	}
	else
	{
		*mode_out = EXP_MARK;
		*key_out = ft_strdup(arg);
		*val_out = NULL;
	}
	if (!*key_out)
		return (-1);
	if ((*mode_out != EXP_MARK) && !*val_out)
		return (-1);
	return (0);
}



void    export_error_identifier(const char *arg)
{
    ft_putstr_fd("minishell: export: `", 2);
    ft_putstr_fd((char *)arg, 2);
    ft_putstr_fd("': not a valid identifier\n", 2);
}

