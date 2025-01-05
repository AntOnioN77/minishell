/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_find_bigest.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 10:13:49 by antofern          #+#    #+#             */
/*   Updated: 2024/08/27 10:16:58 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"
#include <limits.h>


/*Busca el valor mas alto contenido en el anillo*/
int ring_find_bigest(t_ring *ring)
{
	int i;
	int result;

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
