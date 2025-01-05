/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_in_bounds_int.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:12:40 by antofern          #+#    #+#             */
/*   Updated: 2024/09/02 13:14:41 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"
#include <limits.h>

/*Receives a pointer to the first character of a string <char *digits>,
with or within an unique simbol (+-).
Returns TRUE if the number can be represented as an int type.*/
bool	ft_is_in_bounds_int(char *num)
{
	long	unbr;
	size_t	i;
	bool	sign;

	i = 0;
	sign = FALSE;
	if (num[0] == '-' || num[0] == '+' )
	{
		sign = TRUE;
		i++;
	}
	while (ft_isdigit(num[i]))
		i++;
	if ((i - sign) < 1 || i != ft_strlen(num))
		return (FALSE);
	if (ft_strlen(num) > (size_t)(10 + sign))
		return (FALSE);
	unbr = ft_atol(num);
	if (unbr > INT_MAX || unbr < INT_MIN)
		return (FALSE);
	return (TRUE);
}
