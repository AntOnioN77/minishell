#ifndef EXECUTOR_H

# define EXECUTOR_H

# include <ctype.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdarg.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <errno.h>
# include <fcntl.h>
# include "minishell.h"

//Crear estructura para almacenar las variables de ejecución

int	free_null_arr(char ***s);

void	free_arr(char **s);

e_errors create_child(t_task *task, t_environ *environ , int in, int out);

e_errors executor(t_tree *node, t_environ *environ, int in, int out);

void close_fds(int fd_since);

int	error_exe(int er, char *cmd);

e_errors apply_redirs(t_redir *redir, char **word_fail);

void ft_free_null(void **pnt);

//solo test BORRAR
void test_fds(char *where);

#endif
