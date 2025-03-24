/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-r <jperez-r@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 21:21:00 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/24 21:22:14 by jperez-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

void	ft_free_null(void **pnt)
{
	if (pnt == NULL || *pnt == NULL)
		return ;
	free(*pnt);
	*pnt = NULL;
	return ;
}

int	free_null_arr(char ***s)
{
	int		i;
	char	**ptr;

	i = 0;
	if (!s || !*s || !**s)
		return (0);
	ptr = *s;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
	*s = NULL;
	return (1);
}

void	free_arr(char **s)
{
	int	i;

	i = 0;
	if (!s || !*s)
		return ;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
	return ;
}
