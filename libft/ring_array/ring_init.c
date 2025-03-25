/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 12:02:27 by antofern          #+#    #+#             */
/*   Updated: 2025/03/25 23:34:44 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"
#include <stdlib.h>

t_ring	*ring_init(int size)
{
	t_ring	*circular_array;

	circular_array = (t_ring *)malloc(sizeof(t_ring));
	if (circular_array == NULL)
		return (NULL);
	circular_array->buff = (int *)malloc(sizeof(int) * (size + 1));
	if (circular_array->buff == NULL)
	{
		free(circular_array);
		return (NULL);
	}
	circular_array->head = size / 2 + 1;
	circular_array->tail = size / 2;
	circular_array->fill = 0;
	circular_array->max_fill = size;
	circular_array->slots = size + 1;
	return (circular_array);
}
