
#include "minishell.h"

void	ft_unset_one(char *key, t_environ *environ, int index)
{
	int		i;
	char	*msg;

	index = search_var(environ->envp, key);
	if (index == -1)
		return ;
	if (!ft_strcmp(key, "PWD") || !ft_strcmp(key, "SHLVL")
		|| !ft_strcmp(key, "SHELL") || !ft_strcmp(key, "HOME")
		|| !ft_strcmp(key, "OLDPWD") || !ft_strcmp(key, "?"))
	{
		msg = ft_strjoin("PWD, SHLVL, SHELL, HOME, OLDPWD, ?,",
				"are internal variables, not unsettable.\n");
		ft_putstr_fd(msg, 2);
		free(msg);
		return ;
	}
	free(environ->envp[index]);
	i = 1;
	while (environ->envp[index + i])
	{
		environ->envp[index + i - 1] = environ->envp[index + i];
		i++;
	}
	environ->envp[index + i -1] = NULL;
	environ->next = environ->next - 1;
}

void	ft_unset(char **argv, t_environ *environ)
{
	int	i;

	change_var("?", "0", environ);
	if (!argv || !*argv)
		return ;
	i = 1;
	while (argv[i])
	{
		ft_unset_one(argv[i], environ, 0);
		i++;
	}
}
