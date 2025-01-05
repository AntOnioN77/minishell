/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 12:02:27 by antofern          #+#    #+#             */
/*   Updated: 2024/08/27 12:46:42 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ring.h"
#include <stdlib.h>

/* Alloca espacio para un struct t_ring y para el bufer contenido en el.
La capacidad maxima del array será el parametro size.
El buffer (buff) tiene un tamaño de size+1, para hevitar que algunas funciones
posteriores requieran una logica diferente según el buffer esté lleno o vacío.
`circular_array->head = size / 2 + 1;`
-`size / 2` buscamos alejar head y tail de el final del array, puesto que copiar
	ring[0] en ring[50] es la operacion más costosa posible.
-`+1`ring_add_head() coloca un elemento en head-1, para que con un solo elemento
head y tail sean la misma posicion, necesitamos empezar con head adelantado
(head = tail+1)
Head: elemento en el top (level 0)
Tail: ultimo elemento del array
Fill: cantidad de elementos actuelmente en el array
Max_fill: maximo numero de elementos que pueden ser almacenados en el array
Slots: siempre es igual a max_fill+1, que siempre haya almenos un indice vacío
en el buffer, facilita la implementacion de las funciones que lo manipulan.
El puntero retornado debe ser liverado con ring_free*/
t_ring	*ring_init(int size)
{
	t_ring	*circular_array;

	circular_array = (t_ring *)malloc(sizeof(t_ring));
	if (circular_array == NULL)
		return (NULL);
	circular_array->buff = (int *)malloc(sizeof(int) * (size + 1)); 
	if (circular_array->buff == NULL)
	{
		free(circular_array);
		return (NULL);
	}
	circular_array->head = size / 2 + 1;
	circular_array->tail = size / 2;
	circular_array->fill = 0;
	circular_array->max_fill = size;
	circular_array->slots = size + 1;
	return (circular_array);
}
