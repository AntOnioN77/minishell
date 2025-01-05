/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 10:40:23 by antofern          #+#    #+#             */
/*   Updated: 2024/08/26 13:33:19 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (dstsize > i && dst[i])
	{
		i++;
	}
	while ((i + j + 1) < dstsize && src[j])
	{
		dst[i + j] = src[j];
		j++;
	}
	if ((i + j) < dstsize)
		dst[i + j] = '\0';
	while (src[j])
		j++;
	return (i + j);
}

/*
#include <stdio.h>
#include <bsd/string.h>
int main(void)
{
	char dest[14];
	char *src;
	int i;
	
	i = 0;
	while(i < 3)
		{
			dest[i] = 'a';
			i++;
		}
	dest[i] = '\0';
	src = "hola mundo";
	strlcat(src, NULL, 0);
	printf("%s", dest);
}
*/