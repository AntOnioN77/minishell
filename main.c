
#include "minishell.h"
#include "executor.h"

int g_ctrlc = 0;

//Esta funcion es llamada cuando encontramos un pipe con el nodo a su derecha vacío por ejemplo "ls|(vacio)".
// antes de llamar a esta funcion hay que liberar la t_task vacía.
//El parametro t_tree **right recibe un puntero al elemento pipe->right
//Solicita nueva entrada de usuario, y despliega un nuevo arbol, partiendo del nodo vacío, continuacion del arbol original. 
e_errors	continue_cmd_tree(t_tree **right, char **envp)
{
	char		*line;
	e_errors	error;

	//GESTIONAR SEÑAL AQUI, si ctrl+C es pulsado, dberiamos liberar el arbol y volver a pedir entrada de usuario "mini$hell>"
	line = readline("> ");
	if(!line)
		return (READLINE_FAIL);//pasar codigo de señal??
	if(line[0] == '\0')// si la cadena leida esta vacía, vuelve a pedir entrada
	{
		free(line); //COMPROBARRRR!!!!!!!!!!
		return(continue_cmd_tree(right, envp));
	}
	add_history(line);
//	if(expansor(&line, envp) != ALL_OK)
//		return(ERROR_MALLOC);
	*right = build_tree(line);
	if (*right == NULL)
	{
		free(line);
		perror("build_tree:");
		rl_clear_history();
		return (ERROR_MALLOC);
	}
	(*right)->line_extra = line;
	error = touch_up_tree(*right, envp);
	if (error)
		return(error);
	return (check_tree(*right, envp)); // gestionar retorno
}

e_errors	get_cmd_tree(t_tree **tree, char **envp)
{
	char	 	*line;
	e_errors	error;

	g_ctrlc = 0;
	line = readline("mini$hell> ");
fprintf(stdout, "--------------g_ctrlc: %d\n", g_ctrlc);
fprintf(stdout, "--------------strlen: %zu\n", ft_strlen(line));
fprintf(stdout, "--------------line: %s\n", line);
	if(!line)// || ft_strcmp(line, "\n") > 0)// || (g_ctrlc == 130 && ft_strcmp(line, "\n")))
	{
			fprintf(stdout, "--------------salir3\n");
		/*if(g_ctrlc == 130 && ft_strcmp(line, "\n")) //no sirve, ctrl+c no nos saca del flujo de readline
		{
			g_ctrlc = 0;
			fprintf(stdout, "--------------salir4\n");
			free(line);
			return(E_SIGINT);
		}*/
		return (READLINE_FAIL); //Requerimos pasar señal aqui, si fue una señal la que fallo (errno queda a 0 con ctrl+D pues es una señal EOF perfectamente legal)
	}
	if(g_ctrlc == 130 && ft_strlen(line) == 0)
	{
			fprintf(stdout, "--------------salir1\n");
		g_ctrlc = 0;
		free(line);
		return(E_SIGINT);
	}
	if (ft_strlen(line) >= S_LINE_MAX)
	{
		free(line);
		return(LINE_TOO_LONG);
	}
	if (*line)
	{
		add_history(line);
//		if(expansor(&line, envp) != ALL_OK)
//				return(ERROR_MALLOC);
	}
	*tree = build_tree(line);
	if (*tree == NULL)
	{
		free(line);
		perror("build_tree:");
		rl_clear_history();
		return (ERROR_MALLOC);
	}
	(*tree)->line = line;
	error = touch_up_tree(*tree, envp);
	if (error)
		return(error);
	return (check_tree(*tree, envp)); // gestionar retorno
}

void print_error(char *cmd, char *error_msg) //USADA EN IMPRIMIR ERRORES DE PROCESO HIJO
{
	char *msg_error;

	msg_error = ft_strjoin(cmd, error_msg);
	ft_putstr_fd(msg_error, 2);
	free(msg_error);
}

void ft_perror(int error) //IMPORTANTE: impresion debe ser atomica, un solo write, estoa implementacion es para salir del paso
{
	ft_putstr_fd("minishell: ", 2);
	if(error == SYNTAX_ERROR)
		ft_putstr_fd("syntax error", 2);
//	else if(error == READLINE_FAIL) //Solo deberia llegar aqui por un ctrl+d
//		ft_putstr_fd("exit", 2); //ARREGLAR!!
	else if(error == LINE_TOO_LONG)
		ft_putstr_fd("line too long", 2);
	else
		ft_putnbr_fd(error, 2);
	ft_putchar_fd('\n', 2);//temporal, hacer un solo write
}

e_errors handlerr(e_errors error, t_tree **tree, t_environ *environ)
{
	if (error == ALL_OK)
		return (0);
	if (error == FINISH || error == READLINE_FAIL)
		error = 0;
	else if(error != TASK_IS_VOID && error != CONTINUE)
			ft_perror(error);
	if ( tree && *tree)
	{
		free_tree(*tree);
		*tree = NULL;
	}
	if (g_ctrlc == 130)
		change_var("?", "130", environ);
	if (error== CONTINUE || error == TASK_IS_VOID
		|| error == SYNTAX_ERROR || error == LINE_TOO_LONG || error == E_SIGINT)
	{
		if(error == SYNTAX_ERROR)
			change_var("?", "2", environ);
		else if(error == E_SIGINT)
			change_var("?", "130", environ);
		return (error);//continue
	}

	if(environ)
	{
		free_arr(environ->envp);
		ft_bzero(environ, sizeof(t_environ));
	}
	rl_clear_history();
	close_fds(0);
	exit(error);
}

void shell_cycle(t_tree *tree, t_environ *environ)
{
	int status;
	char *str_status;

	str_status = NULL;
	signal_conf();
	if(handlerr(get_cmd_tree(&tree, environ->envp), &tree, environ))
		return;
	if(handlerr(non_pipable_builtin(tree, environ), &tree, environ))
		return;
 //print_tree(tree, 30);
	if(0 == handlerr(executor(tree, environ, 0, 1), &tree, environ)) //executor deberia simplemente ignorar los builtin no pipeables
	{
			status = wait_all(tree);//, envp);
			if (((((status) & 0x7f) + 1) >> 1) > 0) //aplicamos mascara WIFSIGNALED(status)
				str_status = ft_itoa(((status) & 0x7f) + IS_SIGNAL);//aplicamos mascara WTERMSIG(status) y sumamos 128 (los codigos de señal en bash empiezan en 128)
			else
				str_status = ft_itoa(((status) & 0xff00) >> 8);//aplicamos mascara (WEXISTATUS)
			change_var("?", str_status , environ);
			free(str_status);//NO GESTIONADO POR HANDLE ERROR
			close_fds(3);//SOBRA?????????
			free_tree(tree);
	}
}


int main(int argc, char **argv, char **envp)
{
	t_tree	*tree;
	t_environ environ;

//char *str_bug;

	tree = NULL;
	if (argc != 1 || !argv)
		return(0);
	handlerr(create_envp(envp, &environ), &tree, &environ);
	
	while(1)
	{
//str_bug=ft_strdup("BUG LEAK INTENCIONAL");
//printf("%s\n", str_bug);
//str_bug=NULL;
		shell_cycle(tree, &environ);
//test_fds("196 main.c main");
	}
	return (0);
}
