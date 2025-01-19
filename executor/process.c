#include "minishell.h"
#include "executor.h"

/**
 * Se crea un hijo encargado de ejecutar el comando mientras el padre espera a
 * que finalice para continuar la ejecución
 */
int	create_child(/*t_tree *tree,*/ char **envp, char *path, char **cmd)
{
	pid_t	pid;
	int		stat_loc = 0;

	pid = fork();
	if (pid < 0)
		return (error_exe(2, NULL));
	if (pid == 0)
		execve(path, cmd, envp);
	if (pid != 0)
		waitpid(pid, &stat_loc, 0);
	return (0);
}