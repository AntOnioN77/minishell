/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unquote.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:45:03 by fibo              #+#    #+#             */
/*   Updated: 2025/03/24 23:55:00 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../../libft/headers/libft.h"

char	*unquote_one(char **pntnew, char *pntstr)
{
	size_t	quotlen;

	if ((*pntstr == 39 || *pntstr == '"') && ft_strchr(pntstr +1, *pntstr))
	{
		quotlen = ft_strchr(pntstr +1, *pntstr) - (pntstr +1);
		ft_strlcpy(*pntnew, pntstr +1, quotlen +1);
		*pntnew = *pntnew + quotlen;
		pntstr = ft_strchr(pntstr +1, *pntstr);
	}
	else
	{
		ft_strlcpy(*pntnew, pntstr, 2);
		(*pntnew)++;
	}
	pntstr++;
	return (pntstr);
}

void	unquote(char *str)
{
	char	*newstr;
	char	*pntnew;
	char	*pntstr;

	pntstr = str;
	if (!str || !*str)
		return ;
	newstr = ft_calloc(ft_strlen(pntstr) + 1, sizeof(char));
	pntnew = newstr;
	while (*pntstr)
	{
		pntstr = unquote_one(&pntnew, pntstr);
	}
	ft_strlcpy(str, newstr, 1 + ft_strlen(str));
	free(newstr);
}

void	unquote_task(t_task *node)
{
	int	i;

	unquote(node->cmd);
	unquote(node->redir.infoo);
	unquote(node->redir.outfile);
	i = 0;
	while ((node->argv)[i])
	{
		unquote((node->argv)[i]);
		i++;
	}
	return ;
}