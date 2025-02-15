


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

void print_env(t_environ *environ)
{
	for (int i = 0; i < environ->next; i++)
		fprintf(stderr,"%s\n", environ->envp[i]);
}


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
//		perror("readline:");
		return (READLINE_FAIL);//pasar codigo de señal??
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
//			perror("readline:");
fprintf(stderr,"---------------------linea77 - erno:%d", errno );
			return (READLINE_FAIL); //Requerimos pasar señal aqui, si fue una señal la que fallo (errno queda a 0 con ctrl+D pues es una señal EOF perfectamente legal)
		}
		if (*line)
			add_history(line);
			//save_history(line);
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

/*
void print_error(e_errors error)
{
	//
}
*/

e_errors handlerr(e_errors error, t_tree **tree, t_environ *environ)
{
	//liberar lo liberable
	//error fatal causa exit(err).
	// error de continue, sin error

//fprintf(stderr, "linea 110 variable error:%d\n", (int)error);
	if (error == 0)
		return (0);
	if(environ)
	{
		free_arr(environ->envp);
		ft_bzero(environ, sizeof(t_environ));
	}

	if ( tree && *tree)
	{
		free_tree(*tree);
		*tree = NULL;
	}
		//print_error(error);
	if (error == TASK_IS_VOID || error == SYNTAX_ERROR || error == LINE_TOO_LONG)
	{
		return (error);//continue
	}
	exit(error);
}

int main(int argc, char **argv, char **envp)
{
	t_tree	*tree;
	e_errors		error;
	t_environ environ;
	int status;
	char *str_status;

	error = 0;
	tree = NULL;
	//ft_bzero(&environ, sizeof(t_environ));
	str_status = NULL;
	//Para silenciar warning.
	if (argc != 1 || !argv)
		return(0);
	//load_history();

	error = handlerr(create_envp(envp, &environ), &tree, &environ);


	while(error == 0 || error == TASK_IS_VOID || error == SYNTAX_ERROR)
	{
		signal_conf();
		error = handlerr(get_cmd_tree(&tree, environ.envp), &tree, &environ);
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
			free_null_arr(&environ.envp);
//fprintf(stderr,"SALIDA 136\n");
			return(error);
		}
		error = non_pipable_builtin(tree);//, envp);
		if (error)
		{
			free_tree(tree);
			if(error == FINISH) //NO es un error como tal, built in funcionó
				break ;
printf(" error en non_pipable_built_in: %d\n", error); //solo para pruebas BORRAR
			free_null_arr(&environ.envp);
//fprintf(stderr,"SALIDA 147\n");
			return (error);
		}
//print_tree(tree, 30);
		error = executor(tree, environ.envp, 0, 1); //executor deberia simplemente ignorar los builtin no pipeables cd, export, unset y exit.
//test_fds("main 132");
		if (error == 0)//capturar y gestionar error de executor
		{
           		status = wait_all(tree);//, envp);
				str_status = ft_itoa(((status) & 0xff00) >> 8);
				change_var("?", str_status , &environ);//aplicamos mascara (WEXISTATUS)
				free(str_status);//NO GESTIONADO POR HANDLE ERROR
		}
		else
		{
printf(" error en executor: %d\n", error); //solo para pruebas BORRAR //PERo seria un buen lugar para imprimir con perror 
		}
//test_fds("main 118");
		close_fds(3);
		free_tree(tree);
	}
//fprintf(stderr,"SALIDA 163\n");
//print_env(&environ);
	free_null_arr(&environ.envp);
//print_env(&environ);
	return (error);
}

