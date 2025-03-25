/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_one.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:39:21 by antofern          #+#    #+#             */
/*   Updated: 2025/03/25 13:07:35 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../../libft/headers/libft.h"

t_errors	join_line_buffer(char **newline, char *buffer)
{
	char	*auxline;

	auxline = ft_strjoin(*newline, buffer);
	if (!auxline)
		return (ERROR_MALLOC);
	free(*newline);
	*newline = auxline;
	return (ALL_OK);
}

t_errors	append_dollar_sign(char **newline)
{
	char	*auxline;

	auxline = ft_strjoin(*newline, "$");
	if (!auxline)
		return (ERROR_MALLOC);
	free(*newline);
	*newline = auxline;
	return (ALL_OK);
}

static void	skip_key_length(const char *marker, int *i)
{
	if (marker[*i] == '?')
		(*i)++;
	else
	{
		while (marker[*i] && !ft_strchr(DELIMITERS, marker[*i])
			&& !ft_strchr(WHITESPACES, marker[*i])
			&& !ft_strchr("$\'\"", marker[*i]))
			(*i)++;
	}
}

t_errors	expand_key(char **newline, char *marker, int *i, char **envp)
{
	char	*key;
	char	*value;
	char	*auxline;

	skip_key_length(marker, i);
	key = ft_calloc(sizeof(char), *i);
	if (!key)
		return (ERROR_MALLOC);
	ft_strlcpy(key, &(marker[1]), *i);
	auxline = NULL;
	value = ft_getenv(key, envp);
	auxline = ft_strjoin(*newline, value);
	free(key);
	if (!value)
		return (ALL_OK);
	if (!auxline)
		return (ERROR_MALLOC);
	free(*newline);
	*newline = auxline;
	return (ALL_OK);
}

/*Appends the contents of the buffer to <newline>, expands the variable
pointed to by <marker>. Returns the size of the expansion*/
int	expand_one(char **newline, char *buffer, char *marker, char **envp)
{
	int		i;

	i = 1;
	if (join_line_buffer(newline, buffer))
		return (-1);
	if (ft_strchr(DELIMITERS, marker[i]) || ft_strchr(WHITESPACES, marker[i])
		|| ft_strchr("\'\"", marker[i]))
	{
		if (append_dollar_sign(newline))
			return (-1);
	}
	else
	{
		if (expand_key(newline, marker, &i, envp))
			return (-1);
	}
	return (i);
}
