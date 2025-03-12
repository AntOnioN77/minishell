#include "../executor.h"
#include "../minishell.h"

e_errors ft_echo(t_task *task)
{
	int i;
	char **argv;
	int nflag;

	argv = task->argv;
	if (!argv[0])
		return(FINISH);
	i = 1;
	nflag = 0;
	while(argv[i] && !ft_strcmp("-n", argv[i]))
	{
		nflag = 1;
		i++;	
	}
	while(argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if(argv[i+1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (!nflag)
		ft_putchar_fd('\n', 1);
	return(FINISH);
}

void ft_env(t_environ *environ)
{
	int i;

	i = 0;
	while(environ->envp[i])
	{
		ft_putstr_fd(environ->envp[i], 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
}