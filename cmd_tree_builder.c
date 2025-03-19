#include "minishell.h"
#include "executor.h"

//Esta funcion es llamada cuando encontramos un pipe con el nodo a su derecha vacío por ejemplo "ls|(vacio)".
// antes de llamar a esta funcion hay que liberar la t_task vacía.
//El parametro t_tree **right recibe un puntero al elemento pipe->right
//Solicita nueva entrada de usuario, y despliega un nuevo arbol, partiendo del nodo vacío, continuacion del arbol original. 
e_errors	continue_cmd_tree(t_tree **right, char **envp)
{
	char		*line;
	e_errors	error;

	line = readline("> ");
	if(!line)
		return (READLINE_FAIL);//pasar codigo de señal??
	if(line[0] == '\0')// si la cadena leida esta vacía, vuelve a pedir entrada
	{
		free(line); //COMPROBARRRR!!!!!!!!!!
		return(continue_cmd_tree(right, envp));
	}
	add_history(line);
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
	//fprintf(stderr, "heredoc: %d\n", error);
	if (error)
		return(error);
	return (check_tree(*right, envp)); // gestionar retorno
}

/**
 * Aquí se comprueba si se ha pulsado Ctrl+C sin ninguna ejecución,
 * si se ha pulsado Ctrl+D o ha habido algún fallo con readline
 * y el límite de caracteres por línea
 */
e_errors	line_control(char *line, t_environ *environ)
{
	if (g_ctrlc == 2)
		change_var("?", "130", environ);
	if (!line)
		return (READLINE_FAIL); //Requerimos pasar señal aqui, si fue una señal la que fallo (errno queda a 0 con ctrl+D pues es una señal EOF perfectamente legal)
	if (ft_strlen(line) >= S_LINE_MAX)
	{
		free(line);
		return (LINE_TOO_LONG);
	}
	if (*line)
		add_history(line);
	return (0);
}

e_errors	get_cmd_tree(t_tree **tree, t_environ *environ)
{
	char		*line;
	char		**envp;
	e_errors	error;

	envp = environ->envp;
	g_ctrlc = 0;
	line = readline("mini$hell> ");
	error = line_control(line, environ);
	if (error)
		return (error);
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
		return (error);
	return (check_tree(*tree, envp)); // gestionar retorno
}
