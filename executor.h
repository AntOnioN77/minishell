/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 00:49:23 by fibo              #+#    #+#             */
/*   Updated: 2025/03/25 02:28:12 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <ctype.h>
# include <string.h>
# include <stdarg.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <errno.h>
# include "minishell.h"

int			free_null_arr(char ***s);
enum e_errors	create_child(t_task *task, t_environ *environ, int in, int out);
enum e_errors	executor(t_tree *node, t_environ *environ, int in, int out);
void		close_fds(int fd_since);
enum e_errors	apply_redirs(t_redir *redir, char **word_fail);
void		ft_free_null(void **pnt);

#endif
