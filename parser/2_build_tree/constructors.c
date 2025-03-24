/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:10:13 by antofern          #+#    #+#             */
/*   Updated: 2025/03/24 16:55:54 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_task	*createtask(char *segment, char *end)
{
	t_task	*node;

	node = malloc(sizeof(t_task));
	if (node == NULL)
		return (NULL);
	ft_bzero(node, sizeof(t_task));
	node->type = TASK;
	node->argv = malloc(sizeof(char *) * (count_cmdflags(segment, end) + 1));
	if (node->argv == NULL)
	{
		free(node);
		return (NULL);
	}
	parse_task(segment, end, node);
	return (node);
}

t_tree	*createpipe(char *line, char *pnt)
{
	t_pipe	*node;

	skipwhitesp(&line, pnt);
	node = malloc(sizeof(t_pipe));
	if (node == NULL)
		return (NULL);
	ft_bzero(node, sizeof(t_pipe));
	node->type = PIPE;
	node->left = createtask(line, pnt);
	pnt++;
	if (0 == parsepipe(pnt, &(node->rigth)))
		node->rigth = (t_tree *)createtask(pnt, line + ft_strlen(line));
	return ((t_tree *)node);
}
