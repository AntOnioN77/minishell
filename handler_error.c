
#include "minishell.h"
#include "executor.h"

void	print_error(char *cmd, char *error_msg) //USADA EN IMPRIMIR ERRORES DE PROCESO HIJO
{
	char	*msg_error;

	msg_error = ft_strjoin(cmd, error_msg);
	ft_putstr_fd(msg_error, 2);
	free(msg_error);
}

void	ft_perror(int error)
{
	char	*msg;
	char	*strerr;

	//msg = NULL;
	if (error == SYNTAX_ERROR)
		msg = ft_strdup("minishell: syntax error\n");
	else if (error == LINE_TOO_LONG)
		msg = ft_strdup("minishell: line too long\n");
	else
	{
		strerr = ft_strjoin(ft_itoa(error), "\n");
		msg = ft_strjoin("minishell: ", strerr);
		free(strerr);
	}
	ft_putstr_fd(msg, 2);
	free (msg);
}

e_errors	error_var(e_errors error, t_environ *environ)
{
	if (error == CONTINUE || error == TASK_IS_VOID|| error == SYNTAX_ERROR 
		|| error == LINE_TOO_LONG || error == E_SIGINT)
	{
		if (error == SYNTAX_ERROR)
			change_var("?", "2", environ);
		else if (error == E_SIGINT)
			change_var("?", "130", environ);
		return (error);//continue
	}
	return (0);
}

e_errors	handlerr(e_errors error, t_tree **tree, t_environ *environ)
{
	if (error == ALL_OK)
		return (0);
	if (error == FINISH)
		error = 0;
	else if (error != TASK_IS_VOID && error != CONTINUE
		&& error != READLINE_FAIL && error != E_SIGINT)
		ft_perror(error);
	if (tree && *tree)
	{
		free_tree(*tree);
		*tree = NULL;
	}
	if (error_var(error, environ))
		return (error);
	if (error == READLINE_FAIL)
		error = (unsigned char) ft_atoi(ft_getenv("?", environ->envp));
	if (environ)
	{
		free_arr(environ->envp);
		ft_bzero(environ, sizeof(t_environ));
	}
	rl_clear_history();
	close_fds(0);
	exit(error);
}
