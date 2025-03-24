/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:15:56 by antofern          #+#    #+#             */
/*   Updated: 2025/03/24 16:32:40 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// si str comienza por " y a continuacion hay una nueva ocurrencia de " retorna
// 1 para indicar que se habren comillas dobles. si str comienza por " pero ya
// no hay mas a continuacion, retorna 0 si str comienza por algo distinto a "
// retorna original_flag, para no modificar el estado.
int	is_quoted(char *str, int original_flag)
{
	if (original_flag == 0)
	{
		if (*str == '"' && ft_strchr(str + 1, '"'))
			return (2);
		else if (*str == 39 && ft_strchr(str + 1, 39))
			return (1);
		else
			return (0);
	}
	else if (original_flag == 1)
	{
		if (*str == 39)
			return (0);
		else
			return (1);
	}
	else if (original_flag == 2)
	{
		if (*str == '"')
			return (0);
		else
			return (2);
	}
	return (original_flag);
}

int	is_expansible(char *str)
{
	int	quotes;

	quotes = 0;
	if (!str)
		return (0);
	while (*str)
	{
		quotes = is_quoted(str, quotes);
		if (*str == '$')
			return (1);
		if (*str == 39 && ft_strchr(str + 1, 39) && quotes != 2)
			str = ft_strchr(str + 1, 39);
		str++;
	}
	return (0);
}

int	count_expansions(t_task *node)
{
	int	count;
	int	i;

	count = 1;
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

char	*getkey(char *var)
{
	size_t	len;
	char	*key;

	len = ft_strchr(var, '=') - var;
	key = ft_substr(var, 0, len);
	return (key);
}
