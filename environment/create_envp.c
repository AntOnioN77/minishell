/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 21:03:30 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/25 03:07:59 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

int	count_to_null(void **pnt)
{
	int	i;

	i = 0;
	while (pnt[i] != NULL)
		i++;
	return (i);
}

t_err	copy_prev_envp(char **original, t_environ *environ)
{
	int		i;
	char	*var;

	i = 0;
	while (original[i] != NULL)
	{
		var = ft_strdup(original[i]);
		if (var == NULL)
			return (errno);
		environ->envp[i] = var;
		i++;
	}
	environ->next = i;
	return (0);
}

t_err	create_envp(char **original, t_environ *environ)
{
	int	count;
	int	error;

	ft_bzero(environ, sizeof(t_environ));
	if (original == NULL || *original == NULL)
	{
		environ->envp = ft_calloc(12, sizeof(char *));
		if (environ->envp == NULL)
			return (errno);
		environ->alloced = 12;
	}
	else
	{
		count = count_to_null((void **)original);
		environ->envp = ft_calloc(count * 2, sizeof(char *));
		if (environ->envp == NULL)
			return (errno);
		environ->alloced = count * 2;
		error = copy_prev_envp(original, environ);
	}
	error = init_envp(environ);
	return (error);
}
