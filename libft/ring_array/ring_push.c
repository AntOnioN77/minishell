/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_push.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 10:11:46 by antofern          #+#    #+#             */
/*   Updated: 2025/03/24 23:35:20 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"
#include "../headers/libft.h"

int	ring_push(t_ring *from, t_ring *to)
{
	if (from->fill < 1)
		return (ERROR);
	if (to->max_fill == to->fill)
		return (ERROR);
	ring_add_head(to, ring_get_head(from));
	ring_pop_head(from);
	return (0);
}
