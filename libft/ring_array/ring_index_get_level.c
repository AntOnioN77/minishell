/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_index_get_level.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:48:04 by antofern          #+#    #+#             */
/*   Updated: 2024/08/26 13:48:23 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"

/*Devuelve el nivel al que corresponde un indice del buffer en el array circular.
El nivel inicial es 0. Un nivel de -1  indica que la pila no contiene ningun elemento en esa posicion.*/
int ring_index_get_level(t_ring *ring, int index)
{
	int result;

	result = (index - ring->head + ring->slots) % ring->slots;
	if ((result +1) > ring->fill)
		return (-1);
	return (result);
}
