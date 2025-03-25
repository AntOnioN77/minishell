/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 21:14:05 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/25 02:28:12 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include <limits.h>
#include <sys/stat.h>
#include <stdio.h>

enum e_errors	exec_pipe(t_pipe *pipe_node, t_environ *environ, int in)
{
	int			pipefd[2];
	enum e_errors	err;

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

enum e_errors	executor(t_tree *node, t_environ *environ, int in, int out)
{
	t_pipe		*pipe_node;
	t_task		*task;
	enum e_errors	err;

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
