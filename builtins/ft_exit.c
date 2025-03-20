
#include "minishell.h"

int	select_exitcode(t_task *task, t_environ *environ, char *argument)
{
	ft_putstr_fd("exit\n", 1);
	if (countargs(task) == 2)
	{
		if (*argument != '\0')
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
			return (2);
		}
		else
			return (ft_atoi(task->argv[1]));
	}
	else
		return (ft_atoi(ft_getenv("?", environ->envp)));
}

//libera todo y termina minishell con exit(), solo retorna en caso de error
void	ft_exit(t_task *task, t_tree *tree, t_environ *environ)
{
	int				exitcode;
	unsigned char	truncated;
	char			*argument;

	argument = NULL;
	if (countargs(task) > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		change_var("?", "1", environ);
		return ;
	}
	if (countargs(task) == 2)
	{
		argument = task->argv[1];
		skipwhitesp(&argument, argument + ft_strlen(argument));
		if (*argument == '+' || *argument == '-')
			argument++;
		while (ft_isdigit(*argument))
			argument++;
	}
	exitcode = select_exitcode(task, environ, argument);
	truncated = (unsigned char) exitcode;
	free_tree(tree);
	free_arr(environ->envp);
	exit(truncated);
}
