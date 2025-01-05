/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_prev_index.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:10:40 by antofern          #+#    #+#             */
/*   Updated: 2024/08/27 13:39:50 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*Recibe <index>, y t_ring.slots. Retorna el indice anterior. Cuando <index> sea 0
retornara el ultimo indice del buffer t_ring.buff, de acuerdo con la lógica circular*/
int ring_prev_index(int index, int slots)
{
	return ((index - 1 + slots) % slots);
}
