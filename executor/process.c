/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-r <jperez-r@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 21:12:48 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/24 21:19:30 by jperez-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../executor.h"
#include <sys/stat.h>

void	close_fds(int fd_since)
{
	struct stat	statbuf;

	while (fstat(fd_since, &statbuf) != -1)
	{
		close(fd_since);
		fd_since++;
	}
}

int	wait_all(t_tree *node)
{
	int		status;
	t_task	*task;
	t_pipe	*pipe_node;

	status = 0;
	if (!node)
		return (0);
	if (node->type == PIPE)
	{
		pipe_node = (t_pipe *) node;
		status = wait_all(pipe_node->rigth);
		wait_all((t_tree *) pipe_node->left);
	}
	else if (node->type == TASK)
	{
		task = (t_task *) node;
		waitpid(task->pid, &status, 0);
		if ((((signed char)(((status) & 0x7f) + 1) >> 1) > 0)
			&& ((status) & 0x7f) == SIGINT)
			g_ctrlc = SIGINT;
		close_fds(3);
	}
	return (status);
}
