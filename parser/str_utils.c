/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:03:40 by antofern          #+#    #+#             */
/*   Updated: 2025/02/05 15:02:52 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int isdelimiter(char c)
{
	if (ft_strchr(DELIMITERS, c))
		return (1) ;
	return (0);
}

void	nullify_delimiters(char *str)
{
    if (!str)
        return;
    while (*str)
    {
		
		if(!skip_quotes(&str, str + ft_strlen(str)))
		{
        	if (isdelimiter(*str))
            	*str = '\0';
			str++;
		}
		
    }
}

void	skipwhitesp(char **segment, char *end)
{
	while (*segment != end && ft_strchr(WHITESPACES , **segment))
	(*segment)++;
	return ;
}

//NO TESTEADA


int skip_quotes(char **strpnt, char *end)
{
	char *tmp;

		tmp = *strpnt;
		while (**strpnt == '"' || **strpnt == 39)
		{
			if(**strpnt == '"' && ft_strnchr((*strpnt) +1, '"', (end - *strpnt))) //sin  +1 strchr encontraría el propio caracter de partida
				*strpnt = 1 + ft_strnchr(*strpnt +1, '"', (end - *strpnt));
			else if(**strpnt == 39 && ft_strnchr(*strpnt +1, 39, (end - *strpnt))) //39 es ' en ascii
				*strpnt = 1 + ft_strnchr(*strpnt +1, 39, (end - *strpnt));
			else
				break ;
		}
		if (*strpnt != tmp)
			return (1);
		return (0);
}

/*	NO TESTEADAA
	-Retorna 1 si encuentra c en str, salvo que esté encapsulado en comillas simples o dobles, o doblemente encapsulado.
	-avanza str hasta la primera coincidencia, si no encuentra chr str permanece igual
	-Probablemente no es util para buscar '\0'*/
int strnchr_outquot(char **str, char *end, char c)
{
	char *strpnt;

	strpnt = *str;
	while(strpnt <= end && *strpnt != '\0')//probablemente !='\0' innecesario
	{
		if(*strpnt == c)
		{
			*str = strpnt;
			return (1);
		}
		if(!skip_quotes(&strpnt, end))
			strpnt++;

	}
	return (0);
}