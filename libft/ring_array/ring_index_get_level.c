/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_index_get_level.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:48:04 by antofern          #+#    #+#             */
/*   Updated: 2025/03/24 23:33:54 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"

int	ring_index_get_level(t_ring *ring, int index)
{
	int	result;

	result = (index - ring->head + ring->slots) % ring->slots;
	if ((result +1) > ring->fill)
		return (-1);
	return (result);
}
