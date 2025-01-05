/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_get_head.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:43:29 by antofern          #+#    #+#             */
/*   Updated: 2024/08/26 13:44:13 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"

/*Retorna el valor contenido en el primer elemento del array*/
int ring_get_head(t_ring *ring)
{
	if (ring->fill == 0)
			return (0);
	return (ring->buff[ring->head]);
}
