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
#include <fcntl.h>

//Crear estructura para almacenar las variables de ejecución

int	ft_free_double(char **s);

int create_child(t_task *task, char **envp);

int executor(t_tree *node, char **envp);

void close_fds(int fd_since);

int	error_exe(int er, char *cmd);

e_errors apply_redirs(t_redir *redir);

void ft_free_null(void **pnt);

//solo test BORRAR
void test_fds(char *where);

#endif