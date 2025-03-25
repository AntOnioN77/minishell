/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 20:53:21 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/25 13:04:07 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

t_errors	ft_echo(t_task *task)
{
	int		i;
	char	**argv;
	int		nflag;

	argv = task->argv;
	if (!argv[0])
		return (FINISH);
	i = 1;
	nflag = 0;
	while (argv[i] && !ft_strcmp("-n", argv[i]))
	{
		nflag = 1;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (!nflag)
		ft_putchar_fd('\n', 1);
	return (FINISH);
}

void	ft_env(t_environ *environ)
{
	int	i;

	i = 0;
	while (environ->envp[i])
	{
		ft_putstr_fd(environ->envp[i], 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
}

int	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo") ||!ft_strcmp(cmd, "cd") ||!ft_strcmp(cmd, "pwd")
		||!ft_strcmp(cmd, "export") ||!ft_strcmp(cmd, "unset")
		||!ft_strcmp(cmd, "env") ||!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

t_errors	builtins_exe(t_task *task, t_environ *environ)
{
	char	*cmd;

	(void)environ;
	cmd = task->cmd;
	if (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset"))
	{
		return (FINISH);
	}
	if (!ft_strcmp(cmd, "echo"))
	{
		return (ft_echo(task));
	}
	if (!ft_strcmp(cmd, "pwd"))
	{
		ft_putstr_fd(ft_getenv("PWD", environ->envp), 1);
		ft_putchar_fd('\n', 1);
	}
	if (!ft_strcmp(cmd, "env"))
	{
		ft_env(environ);
	}
	return (FINISH);
}
