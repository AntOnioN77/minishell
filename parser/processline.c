/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:19:13 by antofern          #+#    #+#             */
/*   Updated: 2025/01/25 14:03:56 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//almacena un puntero a la primera letra de esa palabra, consume la siguiente palabra en segment. Si dst es NULL solamente consume un redir en segment
void	getpntword(char **segment, char *end, char **dst)
{
	skipwhitesp(segment, end);
	if (dst != NULL)
		*dst = *segment;
	while (*segment < end)
	{
		if (skip_quotes(segment, end))
			(*segment)++;
		if(!isdelimiter(**segment))
			(*segment)++;
		else
		{
			skipwhitesp(segment, end);
			return ;
		}
	}
	return ;
}

//SEPARAR a getredir.c

// hasta aqui separar

//NO FUNCIONA
int count_cmdflags(char *segment, char *end)
{
	int i;
	char *tmp;

	i = 0;
	while(segment < end)
	{
		get_redir(&segment, end, NULL);// aqui lo usamos solo para consumir las redirecciones, no almacenamos nada
		tmp = segment;
		getpntword(&segment, end, NULL);
		if (tmp != segment)//si getpntword consumio algo, entonces habia una palabra
			i++;
	}

	return(i);
}


//NO TESTEADA
//si falla alocando memoria retorna 1
//get_pathname no se ha implementado aún
//add_arg no implementada
int parse_task(char *segment, char *end, t_task *task)
{
	int i;

	i = 0;
	while(segment < end)
	{
		get_redir(&segment, end, &(task->redir));//si lo primero que encuentra en segment es uno o varios redir los consume, avanzando segment.
		if (!(segment < end))
			break ;
		if(!(task->cmd))
		{
			getpntword(&segment, end, &(task->cmd));//get_word toma la primera palabra que encuentra en segment como comando,la funcion que ejecute debe encargarse de gestionar F_OK X_OK y buscar un path.
			task->argv[0] = task->cmd; // execve por convencion recibe por argv primero el nombre del comando y despues los argumentos con que se ejecuta.
			i++;
		}
		else
		{
			getpntword(&segment, end, &(task->argv[i]));//¿debe comprobar !'\0'. Debe saltarse los espacios vacíos //MIRAR &(task...
			i++;
		}
//printf("task->argv[%d]:%s\n", i-1, task->argv[i-1]);
	}
	task->argv[i] = NULL;
	return (0);
}

int parsepipe(char *line, t_tree **ret)// desde aqui gestionar solo errores de ejecución, no de sintaxis.
{
	char *pnt;

	pnt = line;
	if(!strnchr_outquot(&pnt, pnt + ft_strlen(pnt), '|'))
		return (0);
	*ret = createpipe(line, pnt);
	return (1);
}

t_tree *processline(char *line)//debe retornar un arbol con un nodo para cada fraccion del comando introducido
{
	t_tree *ret;

	if (!ft_strcmp(line, "exit"))// EXIT PROVISIONAL!!!!!!!!
		return (NULL);
	if(0 == parsepipe(line, &ret))
		ret = (t_tree *)createtask(line, line + ft_strlen(line));
	if (!ret)
		printf("Error en reserva de memoria, gestionar");//BORRAESTOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//DESCOMENTAR CUANDO SE HAGA	check_tree(ret);
	nullify_delimiters(line); //argumentos y redirecciones son punteros al string original line, esta funcion debe nulificar: whitespaces, |, <, >>, >, <<, y cualquier otro separador
	return (ret);
}
