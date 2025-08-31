#include "expansion.h"

// static char	*get_env_value(const char *var)
// {
// 	char	*val;
//
// 	val = getenv(var);
// 	if (!val)
// 		return ft_strdup("");
// 	return ft_strdup(val);
// }



// search for a variable "VAR" in envp_cpy and return its value
static char *get_env_value(const char *var, char **envp_cpy)
{
    int i = 0;
    size_t var_len = ft_strlen(var);

    while (envp_cpy && envp_cpy[i])
    {
        // compare only up to the '=' sign
        if (ft_strncmp(envp_cpy[i], var, var_len) == 0 && envp_cpy[i][var_len] == '=')
            return ft_strdup(envp_cpy[i] + var_len + 1);
        i++;
    }
    return ft_strdup(""); // not found
}

static char *status_exp(char c, int last_exit)
{
    char *res;

    if (c == '?')
        res = ft_itoa(last_exit);
    else if (c == '$')
        res = ft_itoa(getpid());
    else
        res = ft_strdup("");
    return res;
}

char *expand_token_text(char *txt, char **envp_cpy, int last_exit)
{
    char *result;
    char *pos;

    result = ft_strdup("");
    if (!result)
        return NULL;
    pos = txt;

    while (*pos)
    {
        if (*pos == '$' && *(pos + 1))
        {
            char *val;
            if (*(pos + 1) == '?' || *(pos + 1) == '$')
            {
                val = status_exp(*(pos + 1), last_exit);
                pos += 2;
            }
            else
            {
                size_t var_len = 0;
                while (pos[1 + var_len] && (ft_isalnum(pos[1 + var_len]) || pos[1 + var_len] == '_'))
                    var_len++;
                if (var_len == 0)
                {
                    val = ft_strdup("$");
                    pos++;
                }
                else
                {
                    char *var_name = ft_substr(pos + 1, 0, var_len);
                    val = get_env_value(var_name, envp_cpy);
                    free(var_name);
                    pos += var_len + 1;
                }
            }
            char *tmp = ft_strjoin(result, val);
            free(result);
            free(val);
            result = tmp;
        }
        else
        {
            char tmp[2];
            tmp[0] = *pos;
            tmp[1] = '\0';
            char *tmp2 = ft_strjoin(result, tmp);
            free(result);
            result = tmp2;
            pos++;
        }
    }
    return result;
}

void	expand(t_token *token, char **envp_cpy, int last_exit)
{
	t_token	*current;
	char	*expanded;

	current = token;
	while (current)
	{
		if (current->type == WORD || current->type == DOUBLE_Q)
		{
			expanded = expand_token_text(current->txt, envp_cpy, last_exit);
			if (!expanded)
				return;
			free(current->txt);
			current->txt = expanded;
		}
		current = current->next;
	}
}
