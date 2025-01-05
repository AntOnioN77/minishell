/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 14:01:22 by antofern          #+#    #+#             */
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
	while (quotient != 0)
	{
		quotient = quotient / 10;
		n_chars++;
	}
	return (n_chars);
}

static unsigned int	calc_abs(const int n)
{
	unsigned int	un;

	if (n < 0)
	{
		un = n * -1;
	}
	else
	{
		un = (unsigned int)n;
	}
	return (un);
}

static int	ten_pow(int n)
{
	int	result;

	if (n == 0)
		return (1);
	if (n == 1)
		return (10);
	result = 10;
	while (n-- > 1)
	{
		result = result * 10;
	}
	return (result);
}

void	ft_putnbr_fd(int n, int fd)
{
	int				n_chars;
	char			character;
	unsigned int	un;
	int				divisor;

	if (n < 0)
		write(fd, "-", 1);
	if (n == 0)
	{
		write(fd, "0", 1);
		return ;
	}
	n_chars = countchr(n);
	un = calc_abs(n);
	while (n_chars > 0)
	{
		divisor = ten_pow(n_chars - 1);
		character = (un / divisor) + '0';
		write(fd, &character, 1);
		un = un % divisor;
		n_chars--;
	}
}
