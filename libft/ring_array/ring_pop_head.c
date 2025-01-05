/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_pop_head.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:49:16 by antofern          #+#    #+#             */
/*   Updated: 2024/08/26 14:15:28 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"
#include "../headers/libft.h"

/*Si la pila queda vacia tras hacer pop `if (ring->fill == 0)`, compensamos que
ring_add_head() añade en buff[head -1], de esta manera logramos que el primer elemento
que se insertase a continuacion sea a la vez head y tail.*/
int	ring_pop_head(t_ring *ring)
{
	if (ring->fill > 0)
	{
		ring->buff[ring->head] = 0;
		ring->head = ring_next_index(ring->head, ring->slots);
		ring->fill--;
		if (ring->fill == 0)
			ring->head = ring_next_index(ring->head, ring->slots);
		return (OK);
	}
	else
		return (EMPTY_STACK);
}
