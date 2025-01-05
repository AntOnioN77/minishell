/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_get_tail.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:45:45 by antofern          #+#    #+#             */
/*   Updated: 2024/08/26 13:46:22 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"

/*Retorna el valor contenido en el último elemento del array*/
int ring_get_tail(t_ring *ring)
{
	if (ring->fill == 0)
			return (0);
	return (ring->buff[ring->tail]);
}
