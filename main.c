
//compilacion  cc -g3 -Wall -Wextra -Werror minishell.c expansor.c -L. -lft -lreadline
#include "minishell.h"
#include "executor.h"

//solo test://///////////////////////////////////
#include <sys/stat.h>
void test_fds(char *where)
{
	int i = 0;
	struct stat statbuf;

	printf("___TEST_FD__\n   %s\n", where);
	while (i < 20)
	{
		if(fstat(i, &statbuf) == -1)
			printf("| fd[%d] 🔴 | ", i);
		else
			printf("| fd[%d] 🟢 | ", i);
		if(i%5==0)
			printf("\n");
		i++;
	}
}
////////////////////////////////////////////////

//Esta funcion es llamada cuando encontramos un pipe con el nodo a su derecha vacío por ejemplo "ls|(vacio)".
// antes de llamar a esta funcion hay que liberar la t_task vacía.
//El parametro t_tree **right recibe un puntero al elemento pipe->right
//Solicita nueva entrada de usuario, y despliega un nuevo arbol, partiendo del nodo vacío, continuacion del arbol original. 
e_errors	continue_cmd_tree(t_tree **right, char **envp)
{
	char 		*line;

	//no se si antes o despues de readline, gestionar señales
	line = readline("> ");
	if(!line)
	{
		perror("readline:");
		return (REDLINE_FAIL);
	}
	if(line[0] == '\0')// si la cadena leida esta vacía, vuelve a pedir entrada
		return(continue_cmd_tree(right, envp));
	*right = processline(line);
	rl_clear_history();
	if (*right == NULL)
	{
		free(line);
		perror("processline:");
		rl_clear_history();
		return (ERROR_MALLOC);
	}
	(*right)->line_extra = line;
	if(expand_vars_tree(*right, envp))
		perror("expandtree:");//esta gestion de error es muy mejorable
	return (check_tree(*right, envp)); // gestionar retorno
}

e_errors	get_cmd_tree(t_tree **tree, char **envp)
{
		char 		*line;

		line = readline("mini$hell> ");
		//(*tree)->line = line;
		if(!line)
		{
			perror("readline:");
			return (REDLINE_FAIL);
		}
		if (*line)
			add_history(line);
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
	char	**new_envp;
	
	//Para silenciar warning.
	if (argc != 1 || !argv)
		return(0);
	error = create_envp(envp, new_envp);

	tree=NULL;
	error = 0;
	while(error == 0 || error == TASK_IS_VOID || error == SYNTAX_ERROR)
	{
		signalConf();
		error = 0;
		error = get_cmd_tree(&tree, envp);
		if (error == TASK_IS_VOID)
		{
			free_tree(tree);
			continue;
		}
		else if(error == SYNTAX_ERROR)
		{
			ft_putstr_fd("Syntax error\n", 2);//bash es mas especifico, quiza hay que darle una vuelta.
			free_tree(tree);
			continue;
		}
		else if (error)
		{
printf("MAIN: error en get_cmd_tree: %d\n", error); //solo para pruebas BORRAR
			free_tree(tree);
			return(error);
		}
		error = non_pipable_builtin(tree);//, envp);
		if (error)
		{
			free_tree(tree);
			if(error == FINISH) //NO es un error como tal, built in funcionó
				break ;
printf(" error en non_pipable_built_in: %d\n", error); //solo para pruebas BORRAR
			
			return (error);
		}
//print_tree(tree, 30);
		error = executor(tree, envp, 0, 1); //executor deberia simplemente ignorar los builtin no pipeables cd, export, unset y exit.
//test_fds("main 132");
		if (error == 0)//capturar y gestionar error de executor
           		wait_all(tree);//, envp);
		else
		{
printf(" error en executor: %d\n", error); //solo para pruebas BORRAR //PERo seria un buen lugar para imprimir con perror 
		}
//test_fds("main 118");
		close_fds(3);
		free_tree(tree);
	}
	return (error);
}

