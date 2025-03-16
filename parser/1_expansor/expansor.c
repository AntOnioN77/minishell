#include "../../minishell.h"
#include "../../libft/headers/libft.h"

/*
int	add_pathname(char **cmd,  t_garbage *garbage, char *envp[])
{
	//usa access para verificar cmd como ruta absoluta
	if ((*cmd)[0] == '/' || strncmp((*cmd), "../", 3) == 0 || strncmp((*cmd), "./", 2) == 0)


	//busca en PATH una ruta valida para cmd
		
		//	si la encuentra:
		//	-libera garbaje (aun no se como va a saber si cmd fue o no fue expandida )
		//	-costruye path/name
	return(0);
}
*/

//puede que innecesaria


static int	prepare_expansion(char **origin, char **new_str, t_garbage *garbage, char *envp[])
{
	int	len;

	len = calculate_expansion_length(*origin, envp);
	if (len < 0)
		return (1);
	*new_str = ft_calloc(len + 1, sizeof(char));
	if (*new_str == NULL)
	 	return (1);
	if (garbage->size <= garbage->current)
	{
		ft_putstr_fd("Bad count on expandstr", 2);//DEBUGEO
	 	return(1);
	}
	garbage->pointers[garbage->current] = *new_str;
	garbage->current++;
	*origin = *new_str;
	(*new_str)[len] = '\0';
	return (0);
}

e_errors	expandstr(char **origin, t_garbage *garbage, char *envp[]) //envp debe recibir el array de strings que hemos creado y sobre el que se reflejan las modificaciones que pueda hacer minishell durante la ejecucion
{
	char	*marker;
	char	*new_str;
	char	*str;

	if(!is_expansible(*origin))
		return (ALL_OK);
	str = *origin;
	if (prepare_expansion(origin, &new_str, garbage, envp))
		return (ERROR_MALLOC);
	marker = str;
	while (*marker)
	{
		if (*marker == '$')
		{
			if(handle_dollar(&new_str, &str, &marker, envp))
				return (ERROR_MALLOC);
			continue ;
		}
		if (*marker == 39 && ft_strchr(marker + 1, 39))
			marker = ft_strchr(marker + 1, 39);
		marker++;
	}
	if (*str)
        ft_strlcpy(new_str, str, marker - str + 1);
    return (ALL_OK);
}


e_errors	expand_task(t_task *node, char *envp[])
{
	int	i;

	node->garb.size = count_expansions(((t_task *)node)) + 1;
	node->garb.pointers = ft_calloc(node->garb.size, sizeof(void *));
	if (node->garb.pointers == NULL)
		return (1);
	(node->garb.pointers)[node->garb.size -1] = NULL;
	
	if (expandstr(&(node->cmd), &(node->garb), envp))
		return (1);
	if (node->redir.insymbol != heredoc)
	{
		if (expandstr(&(node->redir.infoo), &(node->garb), envp))
			return (1);
	}
	if (expandstr(&(node->redir.outfile), &(node->garb), envp))
		return (1);
	i = 0;
	while ((node->argv)[i])
	{
		if (expandstr((&(node->argv)[i]), &(node->garb), envp))
			return (1);
		i++;
	}
	return(0);
}

/*void unquote(char *str)
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
}*/

//FIN TEMPORAL

//Si retorna 1 imprimir perror(errno)
//trabaja en este orden: 1.Expande variables 2.Desenvuelve comillas no anidadas 3.Crea documento temporal para heredoc si fuera necesario
//Nueva funcionalidad (requiere cambio de nombre): ahora tambien escribe en archivo temporal para heredocs
/*e_errors	expand_vars_tree(t_tree *node, char *envp[])
{
	t_task *task;
	t_pipe *pipe;
	e_errors error;

	error = INVALID_TYPE;
	if (node->type == PIPE)
	{
		pipe = (t_pipe *)node;
		error=expand_task(pipe->left, envp);
		if(error)
			return (error);
		unquote_task(((t_task *)pipe->left));
		error=create_herefile(&(pipe->left->redir)); //si hubo error lo anota, sera encontrado por check_tree
		if (error)
			return (error);
		error= expand_vars_tree((t_tree *)pipe->rigth, envp);
	}
	else if (node->type == TASK)
	{
		task = (t_task *)node;
		error = expand_task(task, envp);
		if(error)
			return (error);
		unquote_task(task);
		error = create_herefile(&(task->redir)); //si hubo error lo anota, sera encontrado por check_tree
	}
	return (error);
}*/
