/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 23:57:29 by antofern          #+#    #+#             */
/*   Updated: 2024/04/28 23:58:45 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <limits.h>

static int	ft_is_base_ko(char *base)
{
	int	i;
	int	j;

	i = 0;
	while (base[i] != '\0')
	{
		if (base[i] == '-' || base[i] == '+')
			return (1);
		j = i + 1;
		while (base[j])
		{
			if (base[j] == base[i])
				return (1);
			j++;
		}
		i++;
	}
	if (i < 2)
		return (1);
	return (0);
}

static void	ft_imprime(char *base, int *buf_reminds, int nlength)
{
	int	r;

	while (nlength >= 0)
	{
		r = buf_reminds[nlength];
		write(1, &base[r], 1);
		nlength--;
	}
}

static int	distribute(int *reminds, unsigned int *unbr, unsigned int len_base)
{
	int	ndigits;

	ndigits = 0;
	while (len_base <= *unbr)
	{
		reminds[ndigits] = *unbr % len_base;
		*unbr = *unbr / len_base;
		ndigits++;
	}
	ndigits--;
	return (ndigits);
}

void	ft_putnbr_base(int nbr, char *base)
{
	unsigned int	len_base;
	int				buf_reminds[32];
	int				nlength;
	unsigned int	unbr;

	len_base = 0;
	while (base[len_base])
		len_base++;
	if (ft_is_base_ko(base) == 1)
		return ;
	if (nbr < 0)
	{
		unbr = (-nbr);
		write(1, "-", 1);
	}
	else
		unbr = nbr;
	nlength = distribute(buf_reminds, &unbr, len_base);
	write(1, &base[unbr], 1);
	ft_imprime(base, buf_reminds, nlength);
}
