/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_has_duplicates.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:23:50 by antofern          #+#    #+#             */
/*   Updated: 2025/03/25 23:33:39 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

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
