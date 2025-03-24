/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-r <jperez-r@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 20:56:16 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/24 20:56:48 by jperez-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_key(char *key)
{
	int	i;

	if (!ft_strcmp(key, "") || !ft_strcmp(key, "PWD")
		|| !ft_strcmp(key, "SHLVL") || !ft_strcmp(key, "SHELL")
		|| !ft_strcmp(key, "HOME") || !ft_strcmp(key, "OLDPWD"))
		return (0);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	export_error(char *identifier, t_environ *environ)
{
	char	*atomic;
	char	*aux;

	aux = ft_strjoin("mini$hell: export: '", identifier);
	atomic = ft_strjoin(aux, "': not a valid identifier\n");
	free(aux);
	ft_putstr_fd(atomic, 2);
	free(atomic);
	change_var("?", "1", environ);
}

char	*getvalue(char *var)
{
	char	*start;

	start = ft_strchr(var, '=');
	if (!start)
		return (NULL);
	start++;
	return (ft_strdup(start));
}

void	ft_export(t_task *task, t_environ *environ)
{
	char	*key;
	char	*value;
	int		i;

	i = 1;
	change_var("?", "0", environ);
	while (task->argv[i])
	{
		key = getkey(task->argv[i]);
		if (!validate_key(key))
		{
			export_error(task->argv[i], environ);
			free(key);
			i++;
			continue ;
		}
		value = getvalue(task->argv[i]);
		if (search_var(environ->envp, key) == -1)
			add_var(key, value, environ);
		else
			change_var(key, value, environ);
		free(key);
		free(value);
		i++;
	}
}
