/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunbr_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:05:55 by antofern          #+#    #+#             */
/*   Updated: 2024/08/26 13:33:19 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*______________________IMPORTANT NOTICE!!!____________________________________
-This function is only tested to work in a certain context*/
#include "../headers/libft.h"
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

int	ft_imprime(char *base, int *buf_reminds, int nlength)
{
	int	r;
	int	err;
	int	nlen_aux;

	nlen_aux = nlength;
	while (nlength >= 0)
	{
		r = buf_reminds[nlength];
		err = write(1, &base[r], 1);
		if (err == -1)
			return (-1);
		nlength--;
	}
	return (nlen_aux + 1);
}

static int	distribute(int *reminds, long unsigned int *unbr,
unsigned int len_base)
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

int	ft_putunbr_base(long unsigned int nbr, char *base)
{
	unsigned int	len_base;
	int				buf_reminds[32];
	int				nlength;
	int				total_len;

	len_base = 0;
	while (base[len_base])
		len_base++;
	if (ft_is_base_ko(base) == 1)
		return (-1);
	nlength = distribute(buf_reminds, &nbr, len_base);
	if (write(1, &base[nbr], 1) == -1)
		return (-1);
	total_len = ft_imprime(base, buf_reminds, nlength);
	if (total_len == -1)
		return (-1);
	return (1 + total_len);
}
