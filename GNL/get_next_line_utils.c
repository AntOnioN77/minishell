/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:04:38 by antofern          #+#    #+#             */
/*   Updated: 2024/03/07 09:21:21 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (*s != '\0')
	{
		i++;
		s++;
	}
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	int				i;
	unsigned char	uc;

	uc = (unsigned char)c;
	i = 0;
	while (s[i] && (unsigned char)s[i] != uc)
	{
		i++;
	}
	if (s[i] == uc)
		return ((char *)&s[i]);
	else
		return (NULL);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize == 0)
		return (ft_strlen(src));
	while ((i < (dstsize - 1)) && (src[i]))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}

char	*ft_strdup(const char *s1)
{
	size_t	slen;
	char	*new;
	int		i;

	i = 0;
	slen = ft_strlen(s1) + 1;
	new = (char *)malloc(slen * sizeof(char));
	if (new == NULL)
		return (NULL);
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*newstr;
	size_t	s1size;
	size_t	s2size;

	if (!s1 || !s2)
		return (NULL);
	s1size = ft_strlen(s1);
	s2size = ft_strlen(s2);
	newstr = (char *)malloc(sizeof(char) * (s1size + s2size + 1));
	if (newstr == NULL)
		return (NULL);
	ft_strlcpy(newstr, s1, s1size + 1);
	ft_strlcpy(&newstr[s1size], s2, s2size + 1);
	return (newstr);
}
