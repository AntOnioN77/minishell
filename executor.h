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

//Crear estructura para almacenar las variables de ejecución

int	ft_free_double(char **s);

int	create_child(/*t_tree *tree,*/ char **envp, char *path, char **cmd);

int	error_exe(int er, char *cmd);

int execute(t_tree *tree, char **envp);

#endif