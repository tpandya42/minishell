#include "minishell.h"

static void remove_from_env(char ***envp, const char *key)
{
    int i = 0;
    int j;
    char *equal_pos;

    if (!*envp || !key)
        return;

    while ((*envp)[i])
    {
        equal_pos = ft_strchr((*envp)[i], '=');
        if (equal_pos && ft_strncmp((*envp)[i], key, equal_pos - (*envp)[i]) == 0
            && ft_strlen(key) == (size_t)(equal_pos - (*envp)[i]))
        {
            free((*envp)[i]);
            j = i;
            while ((*envp)[j])
            {
                (*envp)[j] = (*envp)[j + 1];
                j++;
            }
            i--; // stay on current index after shift
        }
        i++;
    }
}

int	my_unset(t_program *program, t_node *node)
{
	int	i;
	char	**args;

	args = node->u_data.cmd.argv;
	if (!args || !args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		remove_from_env(&program->envp_cpy, args[i]);
		i++;
	}
	return (0);
}
