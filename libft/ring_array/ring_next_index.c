/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_next_index.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:39:33 by antofern          #+#    #+#             */
/*   Updated: 2024/08/27 13:40:38 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//ring->buff[index] ... cuando hablamos de index, no nos referimos al lugar que ocupa con respecto a head, sino a la posicion en el buffer clasico que empleamos para "simular el comportamiento circular"
/*Recibe <index>, y t_ring.slots. Retorna el indice siguiente. Cuando <index> sea el ultimo indice válido
del buffer t_ring.buff retornara 0 (primer indice), de acuerdo con la lógica circular*/
int ring_next_index(int index, int slots)
{
	return (index + 1) % slots;
}
