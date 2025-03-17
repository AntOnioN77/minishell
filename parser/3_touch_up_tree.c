#include "../minishell.h"
#include "../libft/headers/libft.h"

char *unquote_one(char **pntnew, char *pntstr)
{
	size_t	quotlen;
	
	if ((*pntstr == 39 || *pntstr == '"') &&  ft_strchr(pntstr +1, *pntstr))
	{
		quotlen = ft_strchr(pntstr +1, *pntstr) - (pntstr +1);
		ft_strlcpy(*pntnew, pntstr +1, quotlen +1);
		*pntnew = *pntnew + quotlen;
		pntstr = ft_strchr(pntstr +1, *pntstr);
	}
	else
	{
		ft_strlcpy(*pntnew, pntstr, 2);
		(*pntnew)++;
	}
	pntstr++;
	return (pntstr);
}

void unquote(char *str)
{
	char *newstr;
	char *pntnew;
	char *pntstr;

	pntstr = str;
    if (!str || !*str)
		return;
	newstr = ft_calloc(ft_strlen(pntstr) + 1, sizeof(char));
	pntnew = newstr;
	while(*pntstr)
	{
		pntstr = unquote_one(&pntnew, pntstr);
	}
	ft_strlcpy(str, newstr, 1 + ft_strlen(str));
	free(newstr);
}

static void unquote_task(t_task *node)
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
	return;
}

static e_errors	handle_pipe_node(t_pipe *pipe, char *envp[])
{
	e_errors error;

	error = expand_task(pipe->left, envp);
	if (error)
		return (error);
	unquote_task((t_task *)pipe->left);
	error = create_heredoc(&(pipe->left->redir));
	if (error)
		return (error);
	error = touch_up_tree((t_tree *)pipe->rigth, envp);
	return (error);
}

static e_errors	handle_task_node(t_task *task, char *envp[])
{
	e_errors error;

	error = expand_task(task, envp);
	if (error)
		return (error);
	unquote_task(task);
	error = create_heredoc(&(task->redir));
	return (error);
}

e_errors	touch_up_tree(t_tree *node, char *envp[])
{
	e_errors error;

	if (node->type == PIPE)
	{
		error = handle_pipe_node((t_pipe *)node, envp);
		return (error);
	}
	else if (node->type == TASK)
	{
		error = handle_task_node((t_task *)node, envp);
		return (error);
	}
	return (INVALID_TYPE);
}
