
//compilacion  cc -g3 -Wall -Wextra -Werror minishell.c expansor.c -L. -lft -lreadline
#include "minishell.h"
#include "executor.h"

e_errors	continue_cmd_tree(t_tree **tree, char **envp)
{
	char 		*line;

	//no se si antes o despues de readline, gestionar señales
	line = readline(">");
	//(*tree)->line_extra = line;
	if(!line)
	{
		perror("readline:");
		return (REDLINE_FAIL);
	}
	*tree = processline(line);
	rl_clear_history();
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
//			free(line);
			perror("processline:");
			rl_clear_history();
			return (ERROR_MALLOC);
		}
		(*tree)->line = line;
		if(expand_vars_tree(*tree, envp))
			perror("expandtree:");//esta gestion de error es muy mejorable
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
<<<<<<< HEAD
		error = get_cmd_tree(&tree, envp);
		if (error)
		{
printf(" error en get_cmd_tree: %d\n", error); //solo para pruebas BORRAR
			free_tree(tree);
			return(error);
		}
		error = non_pipable_builtin(tree);//, envp);
		if (error)
=======
		line = readline("mini$hell>");
		if(!line || ft_strcmp(line, "exit") == 0)
		{
			perror("readline:");
			if (line)
				free(line);
			return (1);
		}
		if (*line)
			add_history(line);
		tree = processline(line);
		if (tree == NULL)
>>>>>>> 1c383a1 (commit for save)
		{
			free_tree(tree);
			if(error == FINISH) //NO es un error como tal, built in funcionó
				break ;
printf(" error en non_pipable_built_in: %d\n", error); //solo para pruebas BORRAR
			
			return (error);
		}
		//print_tree(tree, 30);
		error = executor(tree, envp); //executor deberia simplemente ignorar los builtin no pipeables cd, export, unset y exit.
		if (error == 0)//capturar y gestionar error de executor
           		wait_all(tree);//, envp);
		else
			printf(" error en executor: %d\n", error); //solo para pruebas BORRAR
		free(line);
		free_tree(tree);
	}
	return (error);
}

