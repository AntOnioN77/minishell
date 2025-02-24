/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:10:13 by antofern          #+#    #+#             */
/*   Updated: 2025/02/24 12:50:29 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//Si retorna null es un fallo de ejecución, habria que liberar todo el arbol y lanzar error
t_task *createtask(char *segment, char *end)
{
	t_task *node;

	node = malloc(sizeof(t_task));
	if (node == NULL)
		return (NULL);
	ft_bzero(node, sizeof(t_task));//free_tree() recorre todo el arbol liverando todo deteniendose en NULL, si algún elemento puntero no llegase a ser alocado permanecería muy convenientemente NULL
	node->type = TASK;
	node->argv = malloc(sizeof(char *) * (count_cmdflags(segment, end) + 1));
//	printf("numero de punteros en argv: %d\n", (count_cmdflags(segment, end) + 1));
	if (node->argv == NULL)
	{
		free(node);
		return (NULL);
	}
//Quiza es mejor una sola funcion, que vaya discriminando si la palabra apuntada por segment es un redir un comando o un argumento
//en lugar de parse_cmdflags por un lado y parsepipes por otro
	parse_task(segment, end, node);//no aloca memoria nueva solo completa campos de node. 
	return (node);
}

//NO TESTEADA
//TO DO debe liberar cosas en caso de fallo de algun malloc
//constructor de pipetree, setea izquierda con createtask(line, pnt), y derecha recurriendo a parsepipe(line +1)'
t_tree *createpipe(char *line,char *pnt)
{
	t_pipe *node;

	skipwhitesp(&line, pnt);
//	if (line == pnt)
	node = malloc(sizeof(t_pipe));
	if(node == NULL)
		return (NULL);
	ft_bzero(node, sizeof(t_pipe));
	node->type = PIPE;
	node->left = createtask(line, pnt);
	pnt++;
	if(0 == parsepipe(pnt, &(node->rigth)))//parsepipe retorna 0 si no encontro un |, si lo encontro retorna 1. Si ocurrió un error retorna 1 y pone ret=NULL
		node->rigth = (t_tree *)createtask(pnt, line + ft_strlen(line));// parsetask recibe line al completo
	return((t_tree *)node);
}
