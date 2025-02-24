
#include "../minishell.h"
#include "../libft/headers/libft.h"

void unquote(char *str)
{
	char *newstr;
	char *pntnew;
	int quotlen;
	char *pntstr;

	pntstr = str;
    if (!str || !*str)
		return;
	newstr = ft_calloc(ft_strlen(pntstr) + 1, sizeof(char));
	pntnew = newstr;
	while(*pntstr)
	{
		if ((*pntstr == 39 || *pntstr == '"') &&  ft_strchr(pntstr +1, *pntstr))
		{
			quotlen = ft_strchr(pntstr +1, *pntstr) - (pntstr +1) ;
			ft_strlcpy(pntnew, pntstr +1, quotlen +1);
			pntnew = pntnew + quotlen;
			pntstr = ft_strchr(pntstr +1, *pntstr);
		}
		else
		{
			ft_strlcpy(pntnew, pntstr, 2);
			pntnew++;
		}
		pntstr++;
	}
	ft_strlcpy(str, newstr, 1 + ft_strlen(str));
	free(newstr);
}

static int	unquote_task(t_task *node)
{
	int	i;

	unquote(node->cmd);
	unquote(node->redir.infoo);
	unquote(node->redir.outfile);
	i = 0;
	while ((node->argv)[i])
	{
		unquote((node->argv)[i]);
		i++;
	}
	return(0);
}

//FIN TEMPORAL

//Si retorna 1 imprimir perror(errno)
//trabaja en este orden: 1.Desenvuelve comillas no anidadas 2.Crea documento temporal para heredoc si fuera necesario
//Nueva funcionalidad (requiere cambio de nombre): ahora tambien escribe en archivo temporal para heredocs
e_errors	touch_up_tree(t_tree *node, char *envp[])
{
	t_task *task;
	t_pipe *pipe;
	e_errors error;

	error = INVALID_TYPE;
	if (node->type == PIPE)
	{
		pipe = (t_pipe *)node;
		error=ALL_OK;// expand_task(pipe->left, envp); //cancelamos expand_task para expandir directamente la linea original
		//if(error)
		//	return (error);
		unquote_task(((t_task *)pipe->left));
		error=create_heredoc(&(pipe->left->redir)); //si hubo error lo anota, sera encontrado por check_tree
		if (error)
			return (error);
		error= touch_up_tree((t_tree *)pipe->rigth, envp);
	}
	else if (node->type == TASK)
	{
		task = (t_task *)node;
		error = ALL_OK; //expand_task(task, envp); //cancelamos expand_task para expandir directamente la linea original
		//if(error)
		//	return (error);
		unquote_task(task);
		error = create_heredoc(&(task->redir)); //si hubo error lo anota, sera encontrado por check_tree
	}
	return (error);
}
