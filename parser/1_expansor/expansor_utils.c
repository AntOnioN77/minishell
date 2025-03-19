/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:15:56 by antofern          #+#    #+#             */
/*   Updated: 2025/03/19 13:30:58 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//si str comienza por " y a continuacion hay una nueva ocurrencia de " retorna 1 para indicar que se habren comillas dobles.
//si str comienza por " pero ya no hay mas a continuacion, retorna 0
//si str comienza por algo distinto a " retorna original_flag, para no modificar el estado.
int is_doublequoted(char *str, int original_flag)
{
	if (*str == '"')
	{
		if(ft_strchr(str + 1, '"'))
			return (1);
		else
			return (0);
	}
	return (original_flag);
}

int	is_expansible(char *str)
{
	int doubleq;

	doubleq = 0;
	if(!str)
		return (0);
	while (*str)
	{
		doubleq = is_doublequoted(str, doubleq);
		if (*str == '$')
			return (1);
		if (*str == 39 && ft_strchr(str + 1, 39) && doubleq == 0)
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
	if (ft_strchr(WHITESPACES,*str) || ft_strchr(DELIMITERS,*str))
		return(ft_strdup("$"));
	validvar = ft_strdup("");
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

////consume en str $NOMBRE_DE_VARIABLE y retorna el tamaño del valor apuntado por esa variable
//si ocurrio un error retorna -1
static int	var_expansion_len(char **str, char *envp[])
{
	int len;
	char *keyvar;
	char *valuevar;

	*str = *str + 1; //avanzar para saltarse el '$'
	keyvar = foundvar(*str, envp);
	if(ft_strcmp(keyvar, "$"))
		*str = (*str) + ft_strlen(keyvar);
	valuevar = ft_getenv(keyvar, envp);
	free(keyvar);
	len = ft_strlen(valuevar) +1;

	return (len);
}

int is_closed_quote(char *str)
{
	if ((*str == 39 || *str == '"') &&  ft_strchr(str +1, *str))
		return (1);
	return (0);
}

//consume hasta la siguiente comilla simple.
//Retorna el numero de caracteres consumidos.
//Ignora el primer caracter apuntado por str. No verifica que el primer caracter sea una comilla.
int consume_simplequotes(char **str)
{
	char *aux;

	aux = *str;
	*str = ft_strchr(*str + 1, 39);
	return (*str - aux);
}

//NO SEPARAR DE EXPANDSTR()
int	calculate_expansion_length(char *str, char *envp[])
{
	int len;
	int	ret;
	int doublequot;

	doublequot = 0;
	len = 0;
	while (*str)
	{
		doublequot = is_doublequoted(str, doublequot);
		if (*str == 39 && ft_strchr(str + 1, 39) && doublequot == 0)//compara con flag que indique si estamos dentro de doblequotes (si estas simplequotes estan anidadas no evitan la expansion))
			len += consume_simplequotes(&str);
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
	char *key;
	char *value;

	ft_strlcpy(*new_str, *str, *marker - *str + 1);
	*new_str = *new_str + (*marker - *str);
	(*marker)++;
//fprintf(stderr, "-------169 *maker: %s\n", *marker);
	key = foundvar(*marker, envp);
//fprintf(stderr, "-------171 key: %s\n", key);
	if (ft_strcmp("$", key))
	{
		*marker = *marker + ft_strlen(key);
	}
//fprintf(stderr, "-------169 *maker: %s\n", *marker);
		if (!is_closed_quote(*marker) && (!ft_strcmp("", key) || !ft_strcmp("$", key)))
	{
		**new_str = '$';
		(*new_str)++;
	}
	value = ft_getenv(key, envp);
	free(key);
	*str = *marker;
	ft_strlcpy(*new_str, value, ft_strlen(value) + 1);
	*new_str = *new_str + ft_strlen(*new_str);
	return (0);
}
