/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:15:56 by antofern          #+#    #+#             */
/*   Updated: 2025/03/01 11:29:01 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_expansible(char *str)
{
	if(!str)
		return (0);
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

int	count_expansions(t_task *node)
{
	int	count;
	int	i;
	
	count = 1;//reservamos al menos uno extra para add_pathname(), aunque pueda quedar sin uso en algunos casos.
	count += is_expansible(node->cmd);
	count += is_expansible(node->redir.infoo);
	count += is_expansible(node->redir.outfile);

	i = 0;
	while ((node->argv)[i])
	{
		count += is_expansible((node->argv[i]));
		i++;
	}
	return (count);
}

char *getkey(char *var)
{
	size_t len;
	char *key;

	len = ft_strchr(var, '=') - var;
	key = ft_substr(var, 0, len);
	return(key);
}


char *foundvar(char *str, char *envp[])
{
	char *validvar;
	char *namevar;
	int i;
	size_t len;

	i = 0;
	validvar = ft_strdup("");
	if (strchr(WHITESPACES,*str) || strchr(DELIMITERS,*str))
		return(ft_strdup("$"));
	while(envp[i])
	{
		namevar = getkey(envp[i]);
		if (!namevar)
			return(NULL);
		len = ft_strlen(namevar);
		if(!ft_strncmp(namevar, str, len) && len > ft_strlen(validvar))
		{
			free(validvar);
			validvar = namevar;
		}
		else
			free(namevar);
		i++;
	}
	return(validvar);
	
}


// Si el principio de str coincide con el nombre de alguna variable, retorna strlen(nombre_de variable), en otro caso 0.
// Si varias variables cumplen con este criterio, elegirá la de nombre mas largo.
//ejemplo $PWDdsfsdfsd retorna "3" por PWD.
size_t var_name_len(char *str, char **envp)
{
	char *namevar;
	size_t len;

	namevar = foundvar(str, envp);
	len = ft_strlen(namevar);
	free(namevar);
	return(len);
}

static int	var_expansion_len(char **str, char *envp[])
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
	if (!ft_strcmp(aux,""))
		len++;
	free(aux);
	return len;
}

//NO SEPARAR DE EXPANDSTR()
int	calculate_expansion_length(char *str, char *envp[])
{
	int len;
	int	ret;
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
		else if(*str == '$')
		{
			ret = var_expansion_len(&str, envp);//consume en str $NOMBRE_DE_VARIABLE
			if (ret < 0)
				return (-1);
			len += ret;
			continue ;
		}
		str++;
		len++;
	}
	return (len);
}

int handle_dollar(char **new_str, char **str, char **marker, char *envp[])
{
	char *aux;

	ft_strlcpy(*new_str, *str, *marker - *str + 1);
	*new_str = *new_str + (*marker - *str);
	(*marker)++;
	*str = *marker;
	while (**marker && !ft_strchr(WHITESPACES, **marker)
	&& **marker != '"' && **marker != 39 && **marker != '$')
		(*marker)++;
	aux = ft_substr(*str, 0, *marker - *str);
	if (aux == NULL)
		return (1);
	*str = *marker;
	ft_strlcpy(*new_str, ft_getenv(aux, envp), ft_strlen(ft_getenv(aux, envp)) + 1);
	if (!ft_strcmp(aux, ""))
		**new_str='$';
	free(aux);
	*new_str = *new_str + ft_strlen(*new_str);
	return (0);
}
