/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 01:43:53 by antofern          #+#    #+#             */
/*   Updated: 2025/03/25 13:06:25 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*_________________________TREE_STRUCTS______________________________________*/
/*Este struct no tiene un caso de uso real, funciona como interface. Los
nodos del arbol son en realidad t_line t_pipe t_redir t_task.
El uso de una interface nos permite pasar cualquiera de estos tipos como
argumento a una función. El elemento type, permite a esa función determinar
que tipo de dato ha recibido
en realidad.*/
typedef struct s_tree
{
	enum e_nodes	type;
	char			*line;
	char			*line_extra;//Solo en caso de "ls|(vacio)" line_extra !=NULL
}	t_tree;

/*La linea recibida como task se parsea de izquuierda a derecha, sucesivas
redirecciones se sobreescriben. de manera que hay un solo t_redir por
cada t_task. si no se encuentra ninguna redireccion los enum e_symbols
pemanecen "none"
*/
typedef struct s_redir
{
	enum e_nodes	type;
	enum e_symbols	insymbol;//< o <<
	char			*infoo;// será un archivo para <, O un separator para <<
	enum e_symbols	outsymbol;//>> o >
	char			*outfile;
	char			*tmp_file;
	t_errors		error; //Error causado durante la creación de tmp_file
}	t_redir;

typedef struct s_garbage
{
	void	**pointers;
	int		size;
	int		current;
}	t_garbage;

typedef struct s_task
{
	enum e_nodes	type;
	char			*line;	//solo el primer nodo del arbol contendrá line!=NULL
	char			*line_extra;//Solo en caso de "ls|(vacio)" line_extra !=NULL
	t_redir			redir;
	char			*cmd;
	char			**argv;
	t_garbage		garb;
	int				pid;
}	t_task;

//si left o rigth fuesen NULL indica error de reserva de memoria, hay que
//liverar el arbol entero y lanzar error.
//Un pipe tal que "ls|   (nada)" generaria: pipe->rigth->task, y task tendria
//sus elementos vacios lo cual no es un error, este ejemplo en concreto deja
//a bash pendiente de entrada.
//left siempre debe contener un elemento task, y no puede contener otra cosa
//rigth puede contener otro elemento pipe o un elemento task
typedef struct s_pipe
{
	enum e_nodes	type;
	char			*line;	//solo el primer nodo del arbol contendrá line!=NULL
	char			*line_extra;//Solo en caso de "ls|(vacio)" line_extra !=NULL
	t_task			*left;
	t_tree			*rigth;
}	t_pipe;

/*______________________________Other_Structs____________________________*/
typedef struct s_environ
{
	// gestion de variables de netorno:
	char	**envp;
	int		next;//posicion no inicializada (null) donde podemos escribir la siguiente variable global
	int		alloced; //numero total de posiciones allocadas, de manera que si next+1 < alloced, no sera necesario reallocar memoria
	// gestion de variables locales:
	char	**local;
	int		locnext;
	int		localloced;

//	sighandler_t *sigint_handler;
}	t_environ;
