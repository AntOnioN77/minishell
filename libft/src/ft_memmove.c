/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 19:14:01 by antofern          #+#    #+#             */
/*   Updated: 2024/08/26 13:33:19 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

static void	memcpy_backwards(void *dest, const void *src, size_t n)
{
	int					i;
	unsigned char		*back_dest;
	const unsigned char	*source;

	i = 1;
	source = src;
	back_dest = dest + (n - 1);
	while (back_dest >= (unsigned char *)dest)
	{
		*back_dest = source[n - i];
		back_dest--;
		i++;
	}
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	if (dest > src)
	{
		memcpy_backwards(dest, src, n);
	}
	if (dest < src)
	{
		ft_memcpy(dest, src, n);
	}
	return (dest);
}
