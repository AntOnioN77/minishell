/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_pipable_builtin.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-r <jperez-r@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 20:57:43 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/24 20:58:41 by jperez-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	countargs(t_task *task)
{
	int	i;

	if (!task || !task->argv)
		return (0);
	i = 0;
	while (task->argv[i])
		i++;
	return (i);
}

int	non_pipable_builtin(t_tree *tree, t_environ *environ)
{
	t_task	*task;

	if (tree->type == TASK)
	{
		task = (t_task *)tree;
		if (!((t_task *)tree)->cmd)
			return (ALL_OK);
		if (!ft_strcmp(task->cmd, "cd"))
			ft_cd(task, environ);
		else if (!ft_strcmp(task->cmd, "exit"))
			ft_exit(task, tree, environ);
		else if (!ft_strcmp(task->cmd, "export"))
			ft_export(task, environ);
		else if (!ft_strcmp(task->cmd, "unset"))
			ft_unset(task->argv, environ);
		else
			return (ALL_OK);
		return (CONTINUE);
	}
	return (ALL_OK);
}
