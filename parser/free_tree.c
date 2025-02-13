/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:35:41 by antofern          #+#    #+#             */
/*   Updated: 2025/02/12 14:13:55 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void cleanup_garbage(t_garbage *garbage)
{
	int i;
	
	if (garbage == NULL)
		return ;
	i = 0;
    while (i < garbage->current)
    {
        free(garbage->pointers[i]);
		i++;
    }
    free(garbage->pointers);
	
}

void free_tree(t_tree *node)
{
	t_pipe *pipe_node;
	t_task *task_node;

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
	{
		task_node = (t_task *)node;
		if (task_node->redir.tmp_file)
		{
			unlink(task_node->redir.tmp_file);
			free(task_node->redir.tmp_file);
		}
		cleanup_garbage(&(task_node->garb));
		if (task_node->argv)
			free(task_node->argv);
		free(task_node);
	}
	return ;
}