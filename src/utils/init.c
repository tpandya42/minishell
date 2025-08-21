
//PENDING 42 HEADER

#include "minishell.h"

static char	**ft_dup_env(char **envp)
{
	int i;
	char **envp_cpy;

	i = 0;
	while (envp[i])
		i++;
	envp_cpy = malloc(sizeof(char *) * (i + 1));
	if (!envp_cpy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		envp_cpy[i] = ft_strdup(envp[i]);//check were to free
		if (!envp_cpy[i])
		{
			envp_cpy[i] = NULL;
			free_array(envp_cpy);
			return (NULL);
		}
		i++;
	}
	envp_cpy[i] = NULL;
	return (envp_cpy);
}

static int	init_fd_origin(const char *err_msg, int orig_fd)
{
	int	new_fd;

	new_fd = dup(orig_fd);
	if (new_fd == -1)
		perror(err_msg);
	return (new_fd);
}

static void	handle_init_error(t_program *program)
{
	fprintf(stderr, BOLD RED "Fatal initialization error\n" RESET);
	cleanup_program(program);
	exit(1);
}

void	init_program(t_program *program, char **envp)
{
	program->line = NULL;
	program->envp = envp;
	program->envp_cpy = ft_dup_env(envp);
	program->root = NULL;
	program->token_list = NULL;
	program->fd_in_orig = init_fd_origin
		("dup failes for STDIN init", STDIN_FILENO);//fix pipes
	program->fd_out_orig = init_fd_origin
		("dup failes for STDOUT init", STDOUT_FILENO);//fix pipes
	program->last_exit_status = 0;
	if (!program->envp_cpy)
		malloc_error();
	if (!program->envp_cpy || program->fd_in_orig == -1 
		|| program->fd_out_orig == -1)
		handle_init_error(program);
}
