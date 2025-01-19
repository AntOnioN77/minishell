//Hay buitins que en el contexto de minishell no tiene sentido que formen parte de un pipe
//Son: cd, export, unset y exit.
//la solucion mas directa es ejecutarlos solo cuando el arbol sea unicamente de un nodo

int non_pipable_builtin(tree, envp)
{
	if(tree->type == TASK)
	{
		if(((t_task)tree)->cmd = "cd")
		{
			//to do:lo que quiera que hagamos para cd
			return (1);
		}
		if(((t_task)tree)->cmd = exit)
		{
			//to do:lo que quiera que hagamos para exit
		}
	}
	return (0);
}