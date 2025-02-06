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


void wait_all(t_tree *node)//, char **envp)
{
    int status;
    
    if (!node)
        return;
        
    if (node->type == PIPE)
    {
        t_pipe *pipe_node = (t_pipe *)node;
        wait_all(pipe_node->rigth);//, envp);
        wait_all((t_tree *)pipe_node->left);//, envp);

    }
    else if (node->type == TASK)
    {
        t_task *task = (t_task *)node;
    fprintf(stderr, "Esperando a %s (pid=%d)\n", task->cmd, task->pid);
    if (task->cmd && strcmp(task->cmd, "cat") == 0)
	{
    	fprintf(stderr, "Esperando 1s antes de cat\n");
    	//sleep(1);
	}
	waitpid(task->pid, &status, 0);
    fprintf(stderr, "%s (pid=%d) terminó\n", task->cmd, task->pid);
    }
}
