/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_find_smallest.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 10:18:40 by antofern          #+#    #+#             */
/*   Updated: 2024/08/27 10:21:39 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"
#include <limits.h>

/*Busca el valor mas bajo contenido en el anillo*/
int ring_find_smallest(t_ring *stack)
{
	int i;
	int result;

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
