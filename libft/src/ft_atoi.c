/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:31:32 by antofern          #+#    #+#             */
/*   Updated: 2024/08/26 13:28:57 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

int	ft_atoi(const char *str)
{
	int		total;
	int		sign;

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
