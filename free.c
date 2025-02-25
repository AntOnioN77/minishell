#include <stdlib.h>
#include "minishell.h"

void free_task(t_tree *node)
{
	t_task *task_node;

	task_node = (t_task *)node;
	if (task_node->redir.tmp_file)
	{
		unlink(task_node->redir.tmp_file);
		free(task_node->redir.tmp_file);
	}
	//cleanup_garbage(&(task_node->garb));
	if (task_node->argv)
		free(task_node->argv);
	free(task_node);
}

void free_tree(t_tree *node) //DEMASIADO LARGA
{
	t_pipe *pipe_node;

    if (!node)
		return;
	if(node->line)
		free(node->line);
	if(node->line_extra)
		free(node->line_extra);
	if (node->type == PIPE)
	{
		pipe_node = (t_pipe *)node;
		if (pipe_node->left)
			free_tree((t_tree *)pipe_node->left);
		if (pipe_node->rigth)
			free_tree(pipe_node->rigth);
		free(pipe_node);
	}
    else if (node->type == TASK)
		free_task(node);
	return ;
}

/*
	Libera y nulifica pnt.
	Protege de una liberacion incorrecta si pnt ya esta nulificado
*/
void ft_free_null(void **pnt)
{
	if (pnt == NULL || *pnt == NULL)
		return;
	free(*pnt);
	*pnt = NULL;
	return;
}


/**
 * Libera la memoria reservada en matrices (ej: split) y nullifica
 */
int	free_null_arr(char ***s)
{
	int    i;
	char   **ptr;

	i = 0;
	if (!s || !*s || !**s)
		return (0);
	ptr = *s;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
	*s = NULL;  // Nullificamos el puntero original
	return (1);
}

/**
 * Libera la memoria reservada en matrices (ej: split)
 */
void	free_arr(char **s)
{
	int    i;

	i = 0;
	if (!s || !*s)
		return ;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
	return;
}
