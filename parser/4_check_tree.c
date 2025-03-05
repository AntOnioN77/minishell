
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

int bad_redir(t_redir *redir)
{
	if (redir->insymbol && !redir->infoo)
		return (1);
	if (redir->outsymbol && !redir->outfile)
		return (1);
	return (0);
}

int is_void_task(t_task *task)
{
	char *cmd;
	int i;

	if(task->cmd == NULL && task->redir.insymbol == 0&& task->redir.outsymbol == 0)
		return (1);
	cmd = task->cmd;
	if(!cmd)
		return (1);
	i=0;
	while (cmd[i] && ft_strchr(WHITESPACES, cmd[i]))
		i++;
	if(!cmd[i])
		return (1);
	return(0);
}

int	 check_tree(t_tree *tree, char **envp)
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
		error = check_tree((t_tree *)(pipenode->left), envp);
		if (error == TASK_IS_VOID)
			return (SYNTAX_ERROR);
		else if (error)
			return (error);
		if (!pipenode->rigth)
			return(ERROR_MALLOC);
		error = check_tree(pipenode->rigth, envp);
		if (error == TASK_IS_VOID)
		{
			free_tree(pipenode->rigth);//libera solamente el nodo vacío
			error = continue_cmd_tree(&(pipenode->rigth), envp);//añade un nuevo arbol se construye sobre rama rigth
		}
		if (error)
			return (error);
	}
	else if (tree->type == TASK)
	{
		tasknode = (t_task *)tree;
		if(bad_redir(&(tasknode->redir)))
			return (SYNTAX_ERROR);
		if (is_void_task(tasknode))//NO SIRVE, SI METEN UNA REDIRECCION NADA MAS HACE SIGFAULT
			return (TASK_IS_VOID);
		if(tasknode->redir.error) //se podria retornar sin hacer el if, pero parece mas claro así;
			return(tasknode->redir.error);
	}
	else
		return(INVALID_TYPE);
	return (0);
}
