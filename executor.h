/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:58:13 by antofern          #+#    #+#             */
/*   Updated: 2025/03/25 12:58:18 by antofern         ###   ########.fr       */
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
t_errors	create_child(t_task *task, t_environ *environ, int in, int out);
t_errors	executor(t_tree *node, t_environ *environ, int in, int out);
void		close_fds(int fd_since);
t_errors	apply_redirs(t_redir *redir, char **word_fail);
void		ft_free_null(void **pnt);

#endif
