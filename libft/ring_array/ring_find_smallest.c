/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_find_smallest.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 10:18:40 by antofern          #+#    #+#             */
/*   Updated: 2025/03/24 23:29:39 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"
#include <limits.h>

/*Busca el valor mas bajo contenido en el anillo*/
int	ring_find_smallest(t_ring *stack)
{
	int	i;
	int	result;

	i = 0;
	result = INT_MAX;
	while (i < stack->fill)
	{
		if (ring_get_level(stack, i) < result)
			result = ring_get_level(stack, i);
		i++;
	}
	return (result);
}
