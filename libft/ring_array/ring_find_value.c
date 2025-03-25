/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_find_value.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 10:30:10 by antofern          #+#    #+#             */
/*   Updated: 2025/03/25 13:04:07 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"

t_level	ring_find_value(t_ring *ring, int value)
{
	t_level	i;

	i = 0;
	while (i < ring->fill && ring_get_level(ring, i) != value)
		i++;
	if (ring_get_level(ring, i) != value)
		return (0);
	if ((ring->fill / 2) > i)
		return (i);
	else
		return (-(ring->fill - i));
}
