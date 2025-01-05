/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_rotate.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:45:01 by antofern          #+#    #+#             */
/*   Updated: 2024/08/27 13:11:52 by antofern         ###   ########.fr       */
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
	ring->buff[ring->tail] = ring_get_head(ring);//ring->buff[ring->head];
	ring->head = ring_next_index(ring->head, ring->slots);
	return (OK);
}
