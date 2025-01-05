/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 18:26:39 by antofern          #+#    #+#             */
/*   Updated: 2024/08/26 13:33:19 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	v;

	if (len == 0 && (!needle || !haystack))
		return (NULL);
	i = 0;
	if (needle[0] == '\0')
		return ((char *)haystack);
	while (haystack[i])
	{
		j = i;
		v = 0;
		while (haystack[j] == needle[v] && (j + 1) <= len && haystack[j]
			&& needle[v])
		{
			j++;
			v++;
		}
		if (needle[v] == '\0')
			return ((char *)&(haystack[i]));
		i++;
	}
	return (NULL);
}

/*
#include <string.h>
#include <stdio.h>

int	main(void)
{
	char *result;
	char *haystack = "lorem ipsum";
	char *needle = "ips";
	result = ft_strnstr(haystack, needle, 0);
	printf("%s", result);
	return (0);
}
*/