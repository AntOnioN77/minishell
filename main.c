
#include "minishell.h"
#include "executor.h"

int g_ctrlc = 0;

void	status_control(int status, t_environ *environ)
{
	char	*str_status;

	str_status = NULL;
	if (((((status) & 0x7f) + 1) >> 1) > 0) //aplicamos mascara WIFSIGNALED(status)
		str_status = ft_itoa(((status) & 0x7f) + IS_SIGNAL);//aplicamos mascara WTERMSIG(status) y sumamos 128 (los codigos de señal en bash empiezan en 128)
	else
		str_status = ft_itoa(((status) & 0xff00) >> 8);//aplicamos mascara (WEXISTATUS)
	change_var("?", str_status, environ);
	free(str_status);//NO GESTIONADO POR HANDLE ERROR
}

/*
	la expansion de estos macros, que por norma no podemos usar ensucia el codigo,
	deberiamos simplemente crear funciones para suplirlos:
	WIFSIGNALED(status) se expande en (((signed char) (((status) & 0x7f) + 1) >> 1) > 0)
	WTERMSIG(status) se expande en ((status) & 0x7f)
	WTERMSIG(status)
	WEXISTATUS*/
void	shell_cycle(t_tree *tree, t_environ *environ)
{
	int		status;
	signal_conf();
	if (handlerr(get_cmd_tree(&tree, environ), &tree, environ))
		return ;
	if (handlerr(non_pipable_builtin(tree, environ), &tree, environ))
		return ;
	if (0 == handlerr(executor(tree, environ, 0, 1), &tree, environ)) //executor deberia simplemente ignorar los builtin no pipeables
	{
		status = wait_all(tree);//, envp);
		if (g_ctrlc == SIGINT)
		{
			write(1, "\n", 1);
			change_var("?", "130", environ);
		}
		else 
			status_control(status, environ);
		close_fds(3);//SOBRA?????????
		free_tree(tree);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_tree		*tree;
	t_environ	environ;

	tree = NULL;
	if (argc > 1)
	{
		ft_putstr_fd("Minishell doesn't admit arguments like: ", 2);
		ft_putstr_fd(argv[1], 2);
		return (1);
	}
	handlerr(create_envp(envp, &environ), &tree, &environ);
	while (1)
		shell_cycle(tree, &environ);
	return (0);
}
