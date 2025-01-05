/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 12:59:30 by antofern          #+#    #+#             */
/*   Updated: 2024/01/09 12:59:30 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*memory;

	memory = s;
	while (n-- > 0)
	{
		*memory = (unsigned char)c;
		memory++;
	}
	return (s);
}
