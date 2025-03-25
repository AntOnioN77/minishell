/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_reverse_rotate.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 10:06:15 by antofern          #+#    #+#             */
/*   Updated: 2025/03/25 13:04:07 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"
#include "../headers/libft.h"

int	ring_reverse_rotate(t_ring *ring)
{
	if (ring->fill < 2)
		return (ERROR);
	ring->head = ring_prev_index(ring->head, ring->slots);
	ring->buff[ring->head] = ring_get_tail(ring);
	ring->tail = ring_prev_index(ring->tail, ring->slots);
	return (OK);
}
