/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 13:47:59 by antofern          #+#    #+#             */
/*   Updated: 2024/08/26 13:33:19 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"
#include <stdint.h>
#include <limits.h>

void	*ft_calloc(size_t count, size_t size)
{
	size_t	total_size;
	void	*pnt;

	if (count > 0 && size > (SIZE_MAX / count))
		return (NULL);
	total_size = count * size;
	pnt = (void *)malloc(total_size);
	if (!pnt)
		return (NULL);
	ft_bzero(pnt, total_size);
	return (pnt);
}
