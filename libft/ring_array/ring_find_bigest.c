/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_find_bigest.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 10:13:49 by antofern          #+#    #+#             */
/*   Updated: 2025/03/24 23:30:49 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"
#include <limits.h>

/*Busca el valor mas alto contenido en el anillo*/
int	ring_find_bigest(t_ring *ring)
{
	int	i;
	int	result;

	i = 0;
	result = INT_MIN;
	while (i < ring->fill)
	{
		if (ring_get_level(ring, i) > result)
			result = ring_get_level(ring, i);
		i++;
	}
	return (result);
}
