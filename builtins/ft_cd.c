
#include "minishell.h"
#include <linux/limits.h>

int	cd_destination(t_task *task, char **destination, t_environ *environ)
{
	*destination = task->argv[1];
	if ((*destination)[0] == '~')
	{
		*destination = ft_strjoin(ft_getenv("HOME", environ->envp),
				*destination + 1);
		return (1);
	}
	else if ((*destination)[0] == '-')
	{
		ft_putstr_fd("Mini$ell: No options for cd\n", 2);
		change_var("?", "1", environ);
		return (2);
	}
	return (0);
}

void	ft_cd(t_task *task, t_environ *environ)
{
	char	buffer[PATH_MAX + NAME_MAX + 1];
	char	*destination;
	int		freedest;

	if (countargs(task) != 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		change_var("?", "1", environ);
		return ;
	}
	freedest = cd_destination(task, &destination, environ);
	if (freedest > 1)
		return ;
	if (chdir(destination) == -1)
	{
		perror("minishell: cd:");
		change_var("?", "1", environ);
	}
	else
		change_var("?", "0", environ);
	if (freedest)
		free(destination);
	change_var("OLDPWD", ft_getenv("PWD", environ->envp), environ);
	getcwd(buffer, PATH_MAX + NAME_MAX);
	change_var("PWD", buffer, environ);
}
