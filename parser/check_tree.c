
	//to do: -Si tree es null, ocurrio un error de reserva de
	//memoria en la construccion del arbol


	//to do: si pipe->left o pipe->right son null, ocurrio un error de reserva de
	//memoria en la construccion del arbol

	//to do: si pipe->left existe pero tiene todos los elementos a 0 es un error
	//de sintaxis

	//si pipe->right.type == task, y esa task es valida pero esta vaćia
	//hay que solicitar nueva entrada de usuario y seguir construyendo el arbol
	//si en este punto llega alguna señal como ctrl+c ctrl+d o ctrl+\ hay que gestionarlo

#include "minishell.h"

int	 check_tree(t_tree *tree)
{
	t_pipe *pipenode;
	t_task *tasknode;
	e_errors error;
	
	if(tree == NULL)
	{
		return(ERROR_MALLOC);
	}
	if(tree->type == PIPE)
	{
		pipenode = (t_pipe *)tree;
		if (!pipenode->left)
			return(ERROR_MALLOC);
		error = check_tree(pipenode->left);
		if (error == TASK_IS_VOID)
			return (SYNTAX_ERROR);
		else if (error)
			return (error);
		if (!pipenode->left)
			return(ERROR_MALLOC);
		error = check_tree(pipenode->rigth);
		if (error)
			return (error);
	}
	else if (tree->type == TASK)
	{
		return(check_task)
	}


}
