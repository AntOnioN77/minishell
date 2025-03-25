/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_tree_builder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 20:44:27 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/25 02:28:12 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

enum e_errors	continue_cmd_tree(t_tree **right, char **envp)
{
	char		*line;
	enum e_errors	error;

	line = readline("> ");
	if (!line)
		return (READLINE_FAIL);
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

enum e_errors	line_control(char *line, t_environ *environ)
{
	if (g_ctrlc == 2)
		change_var("?", "130", environ);
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

enum e_errors	get_cmd_tree(t_tree **tree, t_environ *environ)
{
	char		*line;
	char		**envp;
	enum e_errors	error;

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
