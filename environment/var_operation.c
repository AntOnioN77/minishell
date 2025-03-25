/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_operation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 21:02:47 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/25 01:33:46 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

void	*custom_realloc(void **pnt, size_t oldsize, size_t newsize)
{
	void	*newpnt;

	if (*pnt == NULL)
		return (NULL);
	if (oldsize >= newsize)
		return (*pnt);
	newpnt = ft_calloc(1, newsize);
	if (newpnt == NULL)
		return (NULL);
	ft_memcpy(newpnt, *pnt, oldsize);
	free(*pnt);
	*pnt = newpnt;
	return (newpnt);
}

char	*var_buid(char *key, char *newvalue)
{
	size_t	len;
	char	*newvar;

	len = ft_strlen(key) + ft_strlen(newvalue) + 2;
	newvar = ft_calloc(len, sizeof(char));
	if (newvar == NULL)
		return (NULL);
	ft_strlcpy(newvar, key, len);
	ft_strlcat(newvar, "=", len);
	ft_strlcat(newvar, newvalue, len);
	return (newvar);
}

e_errors	change_var(char *key, char *newvalue, t_environ *environ)
{
	size_t	keylen;
	char	*newvar;
	int		i;
	char	**envp;

	if (!key || !newvalue || !environ)
		return (1);
	envp = environ->envp;
	keylen = ft_strlen(key);
	newvar = var_buid(key, newvalue);
	if (!newvar)
		return (errno);
	i = 0;
	while (i < environ->next)
	{
		if (ft_strncmp(envp[i], key, keylen) == 0)
		{
			free(envp[i]);
			envp[i] = newvar;
			return (0);
		}
		i++;
	}
	free(newvar);
	return (1);
}

e_errors	add_var(char *key, char *value, t_environ *environ)
{
	size_t	len;
	char	*newvar;

	if (key == NULL || value == NULL || key[0] == '\0')
		return (1);
	if (environ->next >= (environ->alloced - 1))
	{
		if (!custom_realloc((void **) &(environ->envp)
				, environ->alloced * sizeof(char *)
				, environ->alloced * 2 * sizeof(char *)))
			return (errno);
		environ->alloced = environ->alloced * 2;
	}
	len = ft_strlen(key) + ft_strlen(value) + 2;
	newvar = ft_calloc(len, sizeof(char));
	if (newvar == NULL)
		return (errno);
	ft_strlcpy(newvar, key, len);
	ft_strlcat(newvar, "=", len);
	ft_strlcat(newvar, value, len);
	environ->envp[environ->next] = newvar;
	environ->next = environ->next + 1;
	environ->envp[environ->next] = NULL;
	return (0);
}

int	search_var(char **envp, const char *var)
{
	int		pos;
	size_t	len;

	if (!envp)
		return (-1);
	pos = 0;
	while (envp[pos])
	{
		len = ft_strchr(envp[pos], '=') - envp[pos];
		if (ft_strncmp(envp[pos], var, len) == 0 && len == ft_strlen(var))
		{
			return (pos);
		}
		pos++;
	}
	return (-1);
}
