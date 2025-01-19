/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:20:35 by antofern          #+#    #+#             */
/*   Updated: 2025/01/19 20:38:49 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


//compilacion  cc -g3 -Wall -Wextra -Werror minishell.c expansor.c -L. -lft -lreadline
#include "minishell.h"
/*
// Funcion pensada para aligerar lineas del main, no introducida y requiere una revision intensiva
int command_flow(char **envp) //la gestion de errores de esta funcion es muy provisional
{
	char 	*line;
	t_tree	*tree;
	int		error;

	error = 0;
	while(error = 0)
	{
		line = readline("mini$hell>");
		if(!line)
		{
			perror("readline:");
			return (1);
		}
		tree = processline(line);
		if (tree == NULL)
		{
			free(line);
			return (1);
		}
		error = execline(tree, envp);
		if (error) //execline debe liberar los nodos desde las hojas hacia arriba. 
		{
			free(line);
			free_tree(tree);
		}
	}
	return (error);
}

*/


int main(int argc, char **argv, char **envp)
{
	char 	*line;
	t_tree	*tree;
	int		error;
	//char	**new_envp;
	
	//Para silenciar warning.
	if (argc != 1 || !argv)
		return(0);

	error = 0;
	while(error == 0)
	{
		line = readline("mini$hell>");
		if(!line)
		{
			perror("readline:");
			return (1);
		}
		tree = processline(line);

		if (tree == NULL)
		{
			perror("processline:");
			rl_clear_history();
			free(line);
			return (1);
		}
		if(expand_tree(tree, envp))
			perror("expandtree:");//esta gestion de error es muy mejorable
//		check_tree(*tree); // busca errores y pide nueva entrada de usuario en caso de pipe->rigth con todos los elementos '\0'
		print_tree(tree, 30);
		if(non_pipable_builtin(tree, envp))
			continue ;
//		error = execute(tree, envp);
		free(line);
		free_tree(tree);
	}
	return (error);
}



/*---------------------------EJECUTANDO_EL_ARBOL-------------------------------------------------------------------------
ªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªª*/

//int execline(t_tree)//debe ejecutar y liberar cada nodo del arbol, en los nodos typo task, y finalmente liberar el
//nodo inicial recibido.

//int check_tree() debe comprobar que ningun nodo del arbol es null. Los elementos contenidos en un nodo task, si pueden ser null.
//Si alguno de los nodos es NULL libera todo el arbol, la linea y finaliza el programa mostrando un error.