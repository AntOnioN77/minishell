/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 21:02:53 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/25 13:04:07 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>

t_errors	set_shlvl(t_environ *environ)
{
	t_errors	error;
	char		*num;

	if (ft_getenv("SHLVL", environ->envp) == NULL)
		error = add_var("SHLVL", "1", environ);
	else
	{
		num = ft_itoa(ft_atoi(ft_getenv("SHLVL", environ->envp)) + 1);
		error = change_var("SHLVL", num, environ);
		free(num);
	}
	return (error);
}

t_errors	set_pathshell(t_environ *environ, char *path, t_errors error)
{
	char		*pathshell;

	pathshell = ft_strjoin(path, "/minishell");
	if (!error && ft_getenv("SHELL", environ->envp) == NULL)
		error = add_var("SHELL", pathshell, environ);
	else if (!error)
		error = change_var("SHELL", pathshell, environ);
	free(pathshell);
	return (error);
}

t_errors	init_envp(t_environ *environ)
{
	t_errors	error;
	char		path[PATH_MAX];

	if (environ == NULL || environ->envp == NULL)
		return (errno);
	error = set_shlvl(environ);
	if (getcwd(path, PATH_MAX) == NULL)
		return (errno);
	error = set_pathshell(environ, path, error);
	if (!error && (ft_getenv("PWD", environ->envp) == NULL))
		error = add_var("PWD", path, environ);
	else if (!error)
		error = change_var("PWD", path, environ);
	if (!error && ft_getenv("HOME", environ->envp) == NULL)
		error = add_var("HOME", path, environ);
	if (!error && ft_getenv("OLDPWD", environ->envp) == NULL)
		error = add_var("OLDPWD", "", environ);
	else if (!error)
		error = change_var("OLDPWD", "", environ);
	if (!error && ft_getenv("?", environ->envp) == NULL)
		error = add_var("?", "0", environ);
	else if (!error)
		error = change_var("?", "0", environ);
	return (error);
}
