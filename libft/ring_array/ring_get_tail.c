/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_get_tail.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:45:45 by antofern          #+#    #+#             */
/*   Updated: 2025/03/24 23:32:41 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"

/*Retorna el valor contenido en el último elemento del array*/
int	ring_get_tail(t_ring *ring)
{
	if (ring->fill == 0)
		return (0);
	return (ring->buff[ring->tail]);
}
