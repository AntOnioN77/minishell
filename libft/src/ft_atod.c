/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atod.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 14:24:02 by antofern          #+#    #+#             */
/*   Updated: 2024/08/22 10:39:12 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*______________________IMPORTANT NOTICE!!!____________________________________
-This function is only tested to work in a certain context,
may not be valid for a certain range of values ​​or decimals.
-Does not process exponents or scientific notation*/
#include <stdio.h>
#include <stdlib.h>

static double	atod_int(const char *nptr, int *j)
{
	int		i;
	double	result;

	i = *j;
	result = 0;
	while (nptr[i] && i <= 15)
	{
		if (nptr[i] == '.')
		{
			i++;
			break ;
		}
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	*j = i;
	return (result);
}

double	ft_atod(const char *nptr)
{
	double	result;
	double	decimal;
	int		i;
	double	sign;
	int		j;

	i = 0;
	result = 0.0;
	while (nptr[i] == '-' || nptr[i] == '+' )
		i++;
	if (i > 0 && nptr[i - 1] == '-')
		sign = -1.0;
	else
		sign = 1;
	result = atod_int(nptr, &i);
	j = i;
	decimal = atod_int(nptr, &i);
	i = i - j;
	while (i > 0)
	{
		decimal = decimal / 10;
		i--;
	}
	result = result + decimal;
	return (result * sign);
}
