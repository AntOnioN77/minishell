
//compilacion  cc -g3 -Wall -Wextra -Werror minishell.c expansor.c -L. -lft -lreadline
#include "minishell.h"

e_errors	continue_cmd_tree(t_tree **tree, char **envp)
{
	char 		*line;

	line = readline(">");
	//(*tree)->line_extra = line;
	if(!line)
	{
		perror("readline:");
		return (REDLINE_FAIL);
	}
	*tree = processline(line);
	//free(line);
	if (*tree == NULL)
	{
		free(line);
		perror("processline:");
		rl_clear_history();
		return (ERROR_MALLOC);
	}
	(*tree)->line_extra = line;
	if(expand_vars_tree(*tree, envp))
		perror("expandtree:");//esta gestion de error es muy mejorable
//printf("gt_cmd_tree line25: check_tree return %d\n", check_tree(tree, envp));
	return (check_tree(*tree, envp)); // gestionar retorno
}

e_errors	get_cmd_tree(t_tree **tree, char **envp)
{
		char 		*line;

		line = readline("mini$hell>");
		//(*tree)->line = line;

		if(!line)
		{
			perror("readline:");
			return (REDLINE_FAIL);
		}
		*tree = processline(line);
		//free(line);
		if (*tree == NULL)
		{
			free(line);
			perror("processline:");
			rl_clear_history();
			return (ERROR_MALLOC);
		}
		(*tree)->line = line;
		if(expand_vars_tree(*tree, envp))
			perror("expandtree:");//esta gestion de error es muy mejorable
//printf("gt_cmd_tree line25: check_tree return %d\n", check_tree(tree, envp));
		return (check_tree(*tree, envp)); // gestionar retorno
}

int main(int argc, char **argv, char **envp)
{
	t_tree	*tree;
	int		error;
	//char	**new_envp;
	
	//Para silenciar warning.
	if (argc != 1 || !argv)
		return(0);
	tree=NULL;

	error = 0;
	while(error == 0)
	{
		error = get_cmd_tree(&tree, envp);
		if (error)
		{
			printf(" error en main: %d\n", error); //solo para pruebas BORRAR
			free_tree(tree);
			return(error);
		}
printf("main line 50 t_tree tree apunta a:%p\n",tree);
		print_tree(tree, 30);
//		if(non_pipable_builtin(tree, envp))
//			continue ;//to do: liberar lo que sea necesario liberar
//		if (executor(tree, envp) == 0)//capturar y gestionar error de executor
//           		wait_all(tree);
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
