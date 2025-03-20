//Hay buitins que en el contexto de minishell no tiene sentido que formen parte de un pipe
//Son: cd, export, unset y exit.
//la solucion mas directa es ejecutarlos solo cuando el arbol sea unicamente de un nodo

#include "minishell.h"
//#include <linux/limits.h>
#include <limits.h>

int countargs(t_task *task)
{
	int i;

	if (!task || !task->argv)
		return (0);
	i = 0;
	while(task->argv[i])
		i++;
	return (i);
}

/*
static char *getnamevar()
{
	//probablemente ya hay una funcion que hace esto
}
*/



int	non_pipable_builtin(t_tree *tree, t_environ *environ)
{
	t_task	*task;

	if (tree->type == TASK)
	{
		task = (t_task *)tree;
		if (!((t_task *)tree)->cmd)
			return (ALL_OK);
		if (!ft_strcmp(task->cmd, "cd"))
		{
			ft_cd(task, environ);
		}
		else if (!ft_strcmp(task->cmd, "exit"))
		{
			ft_exit(task, tree, environ);
		}
		else if (!ft_strcmp(task->cmd, "export"))
		{
			ft_export(task, environ);
		}
		else if (!ft_strcmp(task->cmd, "unset"))
			ft_unset(task->argv, environ);
		else		//si se cumple alguna de las condiciones retornamos continue, si la task no era un builtin retornamos all ok, para no interrumpir el flujo normal
			return (ALL_OK);
		return (CONTINUE);
	}
	return (ALL_OK);
}
