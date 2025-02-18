


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
	char	*line;
	char	*oldline;

	//GESTIONAR SEÑAL AQUI, si ctrl+C es pulsado, dberiamos liberar el arbol y volver a pedir entrada de usuario "mini$hell>"
	line = readline("> ");
	if(!line)
	{
//		perror("readline:");
		return (READLINE_FAIL);//pasar codigo de señal??
	}
	if(line[0] == '\0')// si la cadena leida esta vacía, vuelve a pedir entrada
	{
		free(line); //COMPROBARRRR!!!!!!!!!!
		return(continue_cmd_tree(right, envp));
	}
//////////////////
	
	add_history(line);
	if (is_expansible(line))
	{
		oldline = line;
		if (expandstr(&line, envp))
			return(ERROR_MALLOC);
		free(oldline);
	}
///////////////////////////


	*right = processline(line);
//	rl_clear_history();
	if (*right == NULL)
	{
		free(line);
		perror("processline:");
		rl_clear_history();
		return (ERROR_MALLOC);
	}
	(*right)->line_extra = line;
	if(process_tree(*right, envp))
		perror("64->expandtree:");//esta gestion de error es muy mejorable
	return (check_tree(*right, envp)); // gestionar retorno
}

e_errors	get_cmd_tree(t_tree **tree, char **envp)
{
		char 	*line;
		char	*oldline;

		line = readline("mini$hell> ");
		//(*tree)->line = line;
		if(!line)
		{
			return (READLINE_FAIL); //Requerimos pasar señal aqui, si fue una señal la que fallo (errno queda a 0 con ctrl+D pues es una señal EOF perfectamente legal)
		}
		if (*line)
		{
			oldline = line;
			add_history(line);
			if (is_expansible(line))
			{
				oldline = line;
				if (expandstr(&line, envp))
					return(ERROR_MALLOC);
				free(oldline);
			}
			//save_history(line);
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
		if(process_tree(*tree, envp))
			perror("92->expandtree:");//esta gestion de error es muy mejorable
		return (check_tree(*tree, envp)); // gestionar retorno
}

/*
void print_error(e_errors error)
{
	//
}
*/
void print_err(int error)
{
	char *err_msg = ft_itoa(error);//SOLUCION TEMPORAL

	ft_putstr_fd(err_msg, 2);
	ft_putchar_fd('\n', 2);//temporal, hacer un solo write
	free(err_msg);
}

e_errors handlerr(e_errors error, t_tree **tree, t_environ *environ)
{

//fprintf(stderr, "linea 110 variable error:%d\n", (int)error);
	if (error == 0)
		return (0);

	print_err(error);
	if ( tree && *tree)
	{
		free_tree(*tree);
		*tree = NULL;
	}
	if (error == TASK_IS_VOID || error == SYNTAX_ERROR || error == LINE_TOO_LONG)
	{
		if(error == SYNTAX_ERROR)
			change_var("?", "2", environ);
		return (error);//continue
	}
	if(environ)
	{
		free_arr(environ->envp);
		ft_bzero(environ, sizeof(t_environ));
	}
	rl_clear_history();
	close_fds(0);//
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
		if (error)
			continue;
		error = handlerr(non_pipable_builtin(tree), &tree, &environ);
		if (error)
			continue ;
// print_tree(tree, 30);
		error = handlerr(executor(tree, environ.envp, 0, 1), &tree, &environ); //executor deberia simplemente ignorar los builtin no pipeables
		if (!error)
		{
           		status = wait_all(tree);//, envp);
				str_status = ft_itoa(((status) & 0xff00) >> 8);//aplicamos mascara (WEXISTATUS)
				change_var("?", str_status , &environ);
				free(str_status);//NO GESTIONADO POR HANDLE ERROR
		}
		else
			continue ;
		close_fds(3);
		free_tree(tree);
	}
	free_null_arr(&environ.envp);
	return (error);
}

