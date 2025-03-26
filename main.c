/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-r <jperez-r@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 20:33:54 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/26 14:49:04 by jperez-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

int	g_ctrlc = 0;

void	status_control(int status, t_environ *environ)
{
	char	*str_status;

	str_status = NULL;
	if (((((status) & 0x7f) + 1) >> 1) > 0)
		str_status = ft_itoa(((status) & 0x7f) + IS_SIGNAL);
	else
		str_status = ft_itoa(((status) & 0xff00) >> 8);
	change_var("?", str_status, environ);
	free(str_status);
}

void	shell_cycle(t_tree *tree, t_environ *environ)
{
	int		status;

	signal_conf();
	if (handlerr(get_cmd_tree(&tree, environ), &tree, environ))
		return ;
	if (handlerr(non_pipable_builtin(tree, environ), &tree, environ))
		return ;
	if (0 == handlerr(executor(tree, environ, 0, 1), &tree, environ))
	{
		status = wait_all(tree);
		if (g_ctrlc == SIGINT)
		{
			write(1, "\n", 1);
			//change_var("?", "130", environ);
		}
		//else
			status_control(status, environ);
		close_fds(3);
		free_tree(tree);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_tree		*tree;
	t_environ	environ;

	tree = NULL;
	if (argc > 1)
	{
		ft_putstr_fd("Minishell doesn't admit arguments like: ", 2);
		ft_putstr_fd(argv[1], 2);
		return (1);
	}
	handlerr(create_envp(envp, &environ), &tree, &environ);
	while (1)
		shell_cycle(tree, &environ);
	return (0);
}
