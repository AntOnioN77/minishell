/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:45:35 by antofern          #+#    #+#             */
/*   Updated: 2025/01/07 00:48:47 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//puede que ionnecesaria
int	is_expansible(char *str)
{
	while (*str)
	{
		if (*str == '$')
			return (1);
		if (*str == 39 && ft_strchr(str + 1, 39))
			str = ft_strchr(str + 1, 39);
		str++;
	}
	return (0);
}

static int	calculate_variable_length(char **str, char *envp[])
{
	int j = 0;
	char *aux;
	int len;

	(*str)++;
	while ((*str)[j] && !ft_strchr(WHITESPACES, (*str)[j])
	&& (*str)[j] != '"' && (*str)[j] != 39 && (*str)[j] != '$') //echo $VAR$VAR es valido.
		j++;
	aux = ft_substr(*str, 0, j);
	if (aux == NULL)
		return (-1);
	*str += j;
	len = ft_strlen(ft_getenv(aux, envp));
	free(aux);
	return len;
}

//NO SEPARAR DE EXPANDSTR()
static int	calculate_expansion_length(char *str, char *envp[])
{
	int len;
	int	j;
	char *aux;

	len = 0;
	while (*str)
	{
			if (*str == 39 && ft_strchr(str + 1, 39))
			{
				aux = str;
				str = ft_strchr(str + 1, 39);
				len += (str - aux);
			}
			if(*str == '$')
			{
				len += calculate_variable_length(&str, envp);//consume en str $NOMBRE_DE_VARIABLE
				continue ;
			}
			str++;
			len++;
	}
	return (len);
}

int	expandstr(char **origin, t_garbage *garbage, char *envp) //envp debe recibir el array de strings que hemos creado y sobre el que se reflejan las modificaciones que pueda hacer minishell durante la ejecucion
{
	char	*marker;
	char	*new_str;
	char	*str;
	char	*aux;
	int		len;

	str = *origin;
	len = calculate_expansion_length(str, envp);
	if (len == ft_strlen(str))
		return (0);

	new_str = ft_calloc(len + 1, sizeof(char));
	if (new_str == NULL)
	 return (1);
	 if (garbage->current >= garbage->size)
	 {
		ft_putstr_fd("Bad count on expandstr", 2);//DEBUGEO
	 	return(1);
	 }
	garbage->pointers[garbage->current] = new_str;
	garbage->current = garbage->current++;
	*origin = new_str;
	new_str[len] = '\0';

	marker = str;
	while (*marker)
	{
		if (*marker == '$')
		{
			ft_strlcpy(new_str, str, marker - str);
			new_str = new_str + (marker - str);
			marker++;
			str = marker;
			while (*marker && !ft_strchr(WHITESPACES, *marker)
			&& *marker != '"' && *marker != 39 && *marker != '$')//
				marker++;
			aux = ft_substr(str, 0, marker - str);
			if (aux == NULL)
				return (1);
			str = marker;
			ft_strlcpy(new_str, ft_getenv(aux, envp), ft_strlen(ft_getenv(aux, envp)));
			free(aux);
			new_str = new_str + ft_strlen(new_str);
		}
		else if (*marker == 39 && ft_strchr(marker + 1, 39))
			marker = ft_strchr(marker + 1, 39);
		else
			marker++;
	}
}

int	expand_task(t_task *node, char *envp)
{
		int	i;

		node->garb.size = count_expansions(((t_task *)node));
		node->garb.pointers = ft_calloc(node->garb.size + 1, sizeof(void *));
		if (node->garb.pointers == NULL)
			return (1);
		(node->garb.pointers)[node->garb.size] == NULL;
		
		if (expandstr(&(node->cmd), &(node->garb), envp))
			return (1);
		if (expandstr(&(node->redir.infoo), $(node->garb), envp))
			return (1);
		if (expandstr(&(node->redir.outfile), $(node->garb), envp))
			return (1);
		i = 0;
		while ((node->argv)[i])
		{
			if (expandstr((&(node->argv)[i]), $(node->garb), envp))
			return (1);
			i++;
		}
}

//Si retorna 1 imprimir perror(errno)
int	expand_tree(t_tree *node, char *envp)
{
	int trace;

	if (node->type == PIPE)
	{
		if(expand_tree(((t_pipe *)node)->left, envp))
			return (1);
		if(expand_tree(((t_pipe *)node)->rigth, envp))
			return (1);
	}
	else if (node->type == TASK)
	{
		if(expand_task(((t_task *)node), envp))
			return (1);
	}
	return (0);
}
