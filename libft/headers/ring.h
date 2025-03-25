/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:25:35 by antofern          #+#    #+#             */
/*   Updated: 2025/03/25 13:07:35 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RING_H
# define RING_H
# include <stddef.h>
# include "libft.h"
# define EMPTY_STACK -1
# define FULL_STACK -2

typedef int	t_level;
typedef struct s_ring
{
	int	*buff;
	int	head;
	int	tail;
	int	slots;
	int	fill;
	int	max_fill;
}	t_ring;

t_ring	*ring_init(int size);
int		ring_next_index(int index, int slots);
int		ring_prev_index(int index, int slots);
int		ring_get_head(t_ring *stack);
int		ring_get_tail(t_ring *stack);
int		ring_pop_head(t_ring *arr);
int		ring_add_head(t_ring *arr, int elem);
int		ring_rotate(t_ring *arr);
int		ring_reverse_rotate(t_ring *arr);
int		ring_swap(t_ring *arr);
int		ring_push(t_ring *from, t_ring *to);
void	ring_free(t_ring *arr);
int		ring_index_get_level(t_ring *arr, int index);
int		ring_get_level(const t_ring *arr, t_level level);
int		ring_find_smallest(t_ring *stack);
int		ring_find_bigest(t_ring *ring);
int		ring_find_smallest(t_ring *stack);
t_level	ring_find_value(t_ring *ring, int value);
t_bool	ring_has_duplicates(t_ring *a);

#endif