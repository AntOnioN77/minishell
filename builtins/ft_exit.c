
#include "minishell.h"

//libera todo y termina minishell con exit(), solo retorna en caso de error
void	ft_exit(t_task *task, t_tree *tree, t_environ *environ)
{
	int				exitcode;
	unsigned char	truncated;
	char			*argument;

	if (countargs(task) > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
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
		if (*argument != '\0')
		{
			ft_putstr_fd("bash: exit: 1 0: numeric argument required\n", 2);
			exitcode = 2;
		}
		else
			exitcode = ft_atoi(task->argv[1]);
	}
	else
		exitcode = ft_atoi(ft_getenv("?", environ->envp));

	truncated = (unsigned char) exitcode;
	free_tree(tree);
	free_arr(environ->envp);
	exit(truncated);
}
