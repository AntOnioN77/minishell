/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_add_head.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:41:40 by antofern          #+#    #+#             */
/*   Updated: 2025/03/25 13:04:07 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"
#include "../headers/libft.h"

int	ring_add_head(t_ring *ring, int new_element)
{
	if (ring->fill >= ring->max_fill)
		return (FULL_STACK);
	ring->head = ring_prev_index(ring->head, ring->slots);
	ring->fill++;
	ring->buff[ring->head] = new_element;
	return (OK);
}
