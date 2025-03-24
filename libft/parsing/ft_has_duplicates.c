/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_has_duplicates.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:23:50 by antofern          #+#    #+#             */
/*   Updated: 2025/03/24 23:28:25 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

/*Busca valores duplicados, si existen retorna 1, en caso contrario retorna 0*/
int	ft_has_duplicates(int n_words, char **array_words)
{
	int	i;
	int	j;

	i = 1;
	while (i < n_words)
	{
		j = i + 1;
		while (j < n_words)
		{
			if (ft_strcmp(array_words[i], array_words[j]) == 0)
			{
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}
