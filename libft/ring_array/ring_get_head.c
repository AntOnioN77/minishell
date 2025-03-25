/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_get_head.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:43:29 by antofern          #+#    #+#             */
/*   Updated: 2025/03/25 23:34:15 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"

int	ring_get_head(t_ring *ring)
{
	if (ring->fill == 0)
		return (0);
	return (ring->buff[ring->head]);
}
