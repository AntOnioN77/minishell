/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_touch_up_tree.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:41:02 by fibo              #+#    #+#             */
/*   Updated: 2025/03/25 03:07:59 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_err	handle_pipe_node(t_pipe *pipe, char *envp[])
{
	t_err	error;

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

static t_err	handle_task_node(t_task *task, char *envp[])
{
	t_err	error;

	error = expand_task(task, envp);
	if (error)
		return (error);
	unquote_task(task);
	error = create_heredoc(&(task->redir));
	return (error);
}

t_err	touch_up_tree(t_tree *node, char *envp[])
{
	t_err	error;

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
