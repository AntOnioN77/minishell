/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_get_head.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:43:29 by antofern          #+#    #+#             */
/*   Updated: 2025/03/24 23:31:25 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"

/*Retorna el valor contenido en el primer elemento del array*/
int	ring_get_head(t_ring *ring)
{
	if (ring->fill == 0)
		return (0);
	return (ring->buff[ring->head]);
}
