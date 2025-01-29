//Hay buitins que en el contexto de minishell no tiene sentido que formen parte de un pipe
//Son: cd, export, unset y exit.
//la solucion mas directa es ejecutarlos solo cuando el arbol sea unicamente de un nodo

#include "minishell.h"

static int countargs(t_task *task)
{
	int i;

	if(!task || !task->argv)
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

int non_pipable_builtin(t_tree *tree)//, char **envp)
{
	if(tree->type == TASK)
	{
		//CD NO ESTA TESTEADO, abstraer funcion cd
		if(((t_task *)tree)->cmd && !ft_strcmp(((t_task *)tree)->cmd, "cd"))
		{	
			if(countargs((t_task *)tree) != 2)
				ft_putstr_fd("minishell: cd: too many arguments", 2);
			else if(chdir(((t_task *)tree)->argv[1]))
				perror("minishell: cd:");
			return (ALL_OK);
		}
		else if(((t_task *)tree)->cmd && !ft_strcmp(((t_task *)tree)->cmd, "exit"))
		{
			free_tree(tree);
			exit(ALL_OK);

		}
		else if(((t_task *)tree)->cmd && !ft_strcmp(((t_task *)tree)->cmd, "export"))
		{
			//TODO:
			/*
			if(envp == NULL)
				//ERROREAR
			if (ft_getenv(getnamevar((t_task *)tree)->argv[1], envp)
			{
				//la variable con ese nombre ya existe, sustituimos
			}
			else
			{
				//creamos la variable
			}
			*/
			printf("export no esta implementado\n");
		}
		else if(((t_task *)tree)->cmd && !ft_strcmp(((t_task *)tree)->cmd, "unset"))
		{
			//TODO
			printf("unset no esta implementado\n");
		}
	}
	return (ALL_OK);
}