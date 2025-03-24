/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_rotate.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:45:01 by antofern          #+#    #+#             */
/*   Updated: 2025/03/24 23:36:16 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"
#include "../headers/libft.h"

// situa el primer elemento (head) en el ultimo lugar
int	ring_rotate(t_ring *ring)
{
	if (ring->fill < 2)
		return (ERROR);
	ring->tail = ring_next_index(ring->tail, ring->slots);
	ring->buff[ring->tail] = ring_get_head(ring);
	ring->head = ring_next_index(ring->head, ring->slots);
	return (OK);
}
