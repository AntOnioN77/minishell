#include "../minishell.h"
#include "../executor.h"
#include <sys/stat.h>


//Empezando en fd_since, cierra tosos los fd abiertos,
//hasta encontrar uno cerrado o inexistente. Antes de cerrar consulta
//si existe y esta abierto con fstat.
void close_fds(int fd_since)
{
	struct stat statbuf;

	while(fstat(fd_since, &statbuf) != -1)
	{
//		printf("cerrando fd %d\n", fd_since);
		close(fd_since);
		fd_since++;
	}
}


int wait_all(t_tree *node)//, char **envp)
{
	int status;

	status = 0;
	if (!node)
		return (0);//????????????
	if (node->type == PIPE)
	{
		t_pipe *pipe_node = (t_pipe *)node;
		status = wait_all(pipe_node->rigth);//, envp);
		wait_all((t_tree *)pipe_node->left);//, envp);
	}
	else if (node->type == TASK)
	{
		t_task *task = (t_task *)node;
		waitpid(task->pid, &status, 0);
		if ((((signed char)(((status) & 0x7f) + 1) >> 1) > 0)
			&& ((status) & 0x7f) == SIGINT)
			g_ctrlc = SIGINT;
		close_fds(3);
	}
	return (status);
}
