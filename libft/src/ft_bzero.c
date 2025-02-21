/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 12:58:39 by antofern          #+#    #+#             */
/*   Updated: 2024/01/09 12:58:39 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*uc;

	if (s == NULL)
		return ;
	uc = (unsigned char *)s;
	while (n > 0)
	{
		*uc = 0;
		uc++;
		n--;
	}
	return ;
}
