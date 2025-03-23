/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 22:19:40 by fibo              #+#    #+#             */
/*   Updated: 2025/03/23 22:19:44 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../../libft/headers/libft.h"

e_errors	join_line_buffer(char **newline, char *buffer)
{
	char	*auxline;

	auxline = ft_strjoin(*newline, buffer);
	if (!auxline)
		return (ERROR_MALLOC);
	free(*newline);
	*newline = auxline;
	return (ALL_OK);
}

e_errors	append_dollar_sign(char **newline)
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

e_errors	expand_key(char **newline, char *marker, int *i, char **envp)
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
/*
static void flush_buffer(char **newline, char *buffer)
{
	char *auxline;

	auxline = ft_strjoin(*newline, buffer);
	if (!auxline)
	{
		free(*newline);
		*newline = NULL;
		return;
	}
	free(*newline);
	*newline = auxline;
	ft_bzero(buffer, BUFFER_SIZE);
}
*/
e_errors handle_expansion(char **marker, char **newline, char *buffer,
char **envp)
{
	int	expanded;

	expanded = expand_one(newline, buffer, *marker, envp);
	if (expanded >= 0)
		*marker += expanded;
	else
	{
		return (ERROR_MALLOC);
	}
	if (!*newline)
		return (ERROR_MALLOC);
	ft_bzero(buffer, BUFFER_SIZE);
	return (ALL_OK);
}

static e_errors	append_to_buffer(char **newline, char *buffer,
	char **marker, int *i)
{
	if (*i == BUFFER_SIZE - 1)
	{
		if (join_line_buffer(newline, buffer))
			return (ERROR_MALLOC);
		*i = 0;
	}
	buffer[*i] = **marker;
	(*i)++;
	(*marker)++;
	return (ALL_OK);
}
e_errors expandstr_motor(char **origin, char **newline, char *buffer,
	char *envp[])
{
	int		quoted;
	char	*marker;
	int		i;

	quoted = 0;
	marker = *origin;
	i = 0;
	while (*marker)
	{
		quoted = is_quoted(marker, quoted);
		if (quoted != 1 && *marker == '$')
		{
			if (handle_expansion(&marker, newline, buffer, envp))
				return (ERROR_MALLOC);
			i = 0;
		}
		else
		{
			if (append_to_buffer(newline, buffer, &marker, &i))
				return (ERROR_MALLOC);
		}
	}
	return (0);
}

e_errors expandstr(char **origin, t_garbage *garbage, char *envp[])
{
	char	buffer[BUFFER_SIZE];
	char	*newline;

	if (!is_expansible(*origin))
		return (ALL_OK);

	ft_bzero(buffer, BUFFER_SIZE);
	newline = ft_strdup("");
	if (!newline)
		return (ERROR_MALLOC);
	if (expandstr_motor(origin, &newline, buffer, envp))
	{
		free(newline);
		return (ERROR_MALLOC);
	}
	if (join_line_buffer(&newline, buffer))
	{
		free(newline);
		return (ERROR_MALLOC);
	}
	garbage->pointers[garbage->current] = newline;
	garbage->current++;
	*origin = newline;
	return (ALL_OK);
}

e_errors	expand_task(t_task *node, char *envp[])
{
	int	i;

	node->garb.size = count_expansions(((t_task *)node)) + 1;
	node->garb.pointers = ft_calloc(node->garb.size, sizeof(void *));
	if (node->garb.pointers == NULL)
		return (1);
	(node->garb.pointers)[node->garb.size -1] = NULL;
	if (expandstr(&(node->cmd), &(node->garb), envp))
		return (1);
	if (node->redir.insymbol != heredoc)
	{
		if (expandstr(&(node->redir.infoo), &(node->garb), envp))
			return (1);
	}
	if (expandstr(&(node->redir.outfile), &(node->garb), envp))
		return (1);
	i = 0;
	while ((node->argv)[i])
	{
		if (expandstr((&(node->argv)[i]), &(node->garb), envp))
			return (1);
		i++;
	}
	return (0);
}
