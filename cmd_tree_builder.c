/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_tree_builder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 20:44:27 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/27 01:29:52 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

t_errors	continue_cmd_tree(t_tree **right, char **envp)
{
	char		*line;
	t_errors	error;

	line = readline("> ");
	if (!line)
	{
		*right = NULL;
		return (CONTINUE);
	}
	if (line[0] == '\0')
	{
		free(line);
		return (continue_cmd_tree(right, envp));
	}
	add_history(line);
	*right = build_tree(line);
	if (*right == NULL)
	{
		free(line);
		perror("build_tree:");
		rl_clear_history();
		return (ERROR_MALLOC);
	}
	(*right)->line_extra = line;
	error = touch_up_tree(*right, envp);
	if (error)
		return (error);
	return (check_tree(*right, envp));
}

t_errors	line_control(char *line, t_environ *environ)
{
	if (g_ctrlc == 2)
	{
		change_var("?", "130", environ);
		g_ctrlc = 0;
	}
	if (!line)
		return (READLINE_FAIL);
	if (ft_strlen(line) >= S_LINE_MAX)
	{
		free(line);
		return (LINE_TOO_LONG);
	}
	if (*line)
		add_history(line);
	return (0);
}

t_errors	get_cmd_tree(t_tree **tree, t_environ *environ)
{
	char		*line;
	char		**envp;
	t_errors	error;

	envp = environ->envp;
	g_ctrlc = 0;
	line = readline("mini$hell> ");
	error = line_control(line, environ);
	if (error)
		return (error);
	*tree = build_tree(line);
	if (*tree == NULL)
	{
		free(line);
		perror("build_tree:");
		rl_clear_history();
		return (ERROR_MALLOC);
	}
	(*tree)->line = line;
	error = touch_up_tree(*tree, envp);
	if (error)
		return (error);
	return (check_tree(*tree, envp));
}
