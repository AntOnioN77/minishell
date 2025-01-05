/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:09:18 by antofern          #+#    #+#             */
/*   Updated: 2024/08/26 13:33:19 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

static int	countchr(int n)
{
	int	n_chars;
	int	quotient;

	quotient = n;
	n_chars = 0;
	if (quotient == 0)
		return (1);
	else if (n < 0)
		n_chars++;
	while (quotient != 0)
	{
		quotient = quotient / 10;
		n_chars++;
	}
	return (n_chars);
}

static unsigned int	calc_abs(const int n, int *sign)
{
	unsigned int	un;

	if (n < 0)
	{
		un = n * -1;
		*sign = -1;
	}
	else
	{
		un = (unsigned int)n;
		*sign = 1;
	}
	return (un);
}

char	*ft_itoa(int n)
{
	int				n_chars;
	char			*result;
	int				sign;
	unsigned int	un;

	n_chars = countchr(n);
	un = calc_abs(n, &sign);
	result = (char *)malloc(sizeof(char) * (n_chars + 1));
	if (result == NULL)
		return (NULL);
	if (un == 0)
		result[0] = '0';
	if (sign == -1)
		result[0] = '-';
	result[n_chars] = '\0';
	while (un != 0)
	{
		n_chars--;
		result[n_chars] = (un % 10) + '0';
		un = un / 10;
	}
	return (result);
}
