/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 14:34:31 by antofern          #+#    #+#             */
/*   Updated: 2024/08/26 13:33:19 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	len;
	char	*result;

	if (!s1)
		return (NULL);
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]) != NULL)
		start++;
	len = start;
	while (s1[len] != '\0')
		len++;
	if (start < len)
		len--;
	while (len > 0 && (ft_strchr(set, s1[len]) != NULL))
		len--;
	len = (len + 1) - start;
	result = ft_substr(s1, start, len);
	return (result);
}
