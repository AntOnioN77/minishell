/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 13:37:34 by antofern          #+#    #+#             */
/*   Updated: 2024/08/26 13:29:27 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

long	ft_atol(const char *str)
{
	long		total;
	long		sign;

	sign = 1;
	total = 0;
	while (*str == '\n' || *str == '\t' || *str == '\r'
		|| *str == '\v' || *str == '\f' || *str == ' ')
		str++;
	if (*str == '-' && (ft_isdigit(str[1]) != 0))
	{
		sign = sign * (-1);
		str++;
	}
	if (*str == '+')
		str++;
	while (ft_isdigit(*str) != 0)
	{
		total = (total * 10) + (*str - '0');
		str++;
	}
	return (total * sign);
}
