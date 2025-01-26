#include "minishell.h"
#include "executor.h"
#include <limits.h>

/**
 * NO ESTA CONTEMPLANDO ERRORES
 * Busca el array que comienza con "PATH" (donde se encuentran las rutas de los
 * ejecutables) y devuelve su posición.
 */
int	search_path(char **envp)
{
	int pos;
	char *subpath;

	pos = 0;
	while (envp[pos])
	{
		subpath = ft_substr(envp[pos], 0, 4);
		if (ft_strcmp(subpath, "PATH") == 0)
		{
			free(subpath);
			return (pos);
		}
		free(subpath);
		pos++;
	}
	return (0);
}

/**
 * NO GESTIONA ERRORES ALLOCANDO MEMORIA (split, search_path, strjoin)
 * Se encarga de comprobar todas las posibles rutas, indicadas en envp, en las
 * que se puede encontrar el comando ejecutable al que se está haciendo
 * referencia.
 * El modo de comprobar si existe e utilizando la función access con los modos
 * F_OK y X_OK que evalúan si el archivo existe y si es ejecutable, 
 * respectivamente.
 */
char	*com_path(char *cmd, char **envp)
{
	char	*path;
	char	*slash;
	char	**enpath;
	int		pos;
	int 	i;

	i = 1;
	pos = search_path(envp);
	enpath = ft_split(envp[pos], ':');
	while (enpath[i])
	{
		slash = ft_strjoin(enpath[i], "/");
		path = ft_strjoin(slash, cmd);
		free(slash);
		if (access (path, F_OK | X_OK) == 0)
		{
			ft_free_double(enpath);
			return (path);
		}
		i++;
		free(path);
	}
	ft_free_double(enpath);
	return (NULL);
}

int create_child(t_task *task, char **envp)
{
	int pid;
	char *pathcmd;
	int		err;

	pid = fork();
	if (pid == -1)
		return (2);
	if (pid == 0)
	{
		close_fds(3);
		pathcmd = com_path(task->cmd, envp);
		if (pathcmd == NULL)
			return (5);//error en reserva de memoria?
		execve(pathcmd, task->argv, envp);
		err = errno;
		close_fds(0);
		free(pathcmd);
		exit(err);
	}
	task->pid = pid;
	return(0);
}

int exec_pipe(t_pipe *pipe_node, char **envp)
{
	int previous_stdin;
    int original_stdout;
	int pipefd[2];
	int	err;

	previous_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	pipe(pipefd);
	if (pipe_node->left)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		err = executor((t_tree *)pipe_node->left, envp);
		if(err != 0)
			return (err);
		dup2(original_stdout, STDOUT_FILENO);
	}
	if (pipe_node->rigth) 
	{
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			err = executor(pipe_node->rigth, envp);
			if(err != 0)
				return (err);
			dup2(previous_stdin, STDIN_FILENO);
	}
	close(previous_stdin);
	close(original_stdout);
	return (0);
}

int executor(t_tree *node, char **envp)
{
	t_pipe *pipe_node;
	t_task *task;
	int	err;

    if (!node)
        return (0);
    
    if (node->type == PIPE)
    {
        pipe_node = (t_pipe *)node;
		err = exec_pipe(pipe_node, envp);
		if (err != 0)
			return (err);
	}
    else if (node->type == TASK)
    {
        task = (t_task *)node;
		err = create_child(task, envp);
		if (err != 0)
			return (err);
    }
    return (0);
}
