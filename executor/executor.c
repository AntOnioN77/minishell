/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 21:14:05 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/25 03:07:59 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include <limits.h>
#include <sys/stat.h>
#include <stdio.h>

t_err	exec_pipe(t_pipe *pipe_node, t_environ *environ, int in)
{
	int			pipefd[2];
	t_err	err;

	pipe(pipefd);
	if (pipe_node->left)
	{
		err = executor((t_tree *)pipe_node->left, environ, in, pipefd[1]);
		if (err != 0)
			return (err);
	}
	if (pipe_node->rigth)
	{
		err = executor(pipe_node->rigth, environ, pipefd[0], STDOUT_FILENO);
		if (err != 0)
			return (err);
	}
	return (0);
}

t_err	executor(t_tree *node, t_environ *environ, int in, int out)
{
	t_pipe		*pipe_node;
	t_task		*task;
	t_err	err;

	if (!node)
		return (0);
	if (node->type == PIPE)
	{
		pipe_node = (t_pipe *)node;
		err = exec_pipe(pipe_node, environ, in);
		if (err != 0)
			return (err);
	}
	else if (node->type == TASK)
	{
		task = (t_task *)node;
		err = create_child(task, environ, in, out);
		if (err != 0)
			return (err);
	}
	return (0);
}
