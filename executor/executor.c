#include "minishell.h"
#include "executor.h"
#include <limits.h>

/**
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
		if (ft_strncmp(subpath, "PATH", ft_strlen(subpath)) == 0)
		{
			free(subpath);
			return (pos);
		}
		free(subpath);
		pos++;
	}
	return (-1);
}

/* 
* Se encarga de comprobar todas las posibles rutas, indicadas en envp, en las
 * que se puede encontrar el comando ejecutable al que se está haciendo
 * referencia.
 * El modo de comprobar si existe e utilizando la función access con los modos
 * F_OK y X_OK que evalúan si el archivo existe y si es ejecutable, 
 * respectivamente.
 */
char	*com_path(char *cmd, char **envp, e_errors *err)
{
	char	*path;
	char	*slash;
	char	**enpath;
	int		pos;
	int 	i;

	if (cmd && (access(cmd, F_OK) == 0))
	{
		path = ft_strdup(cmd);
		if (!path)
			*err = ERROR_MALLOC;
		return (path); //Si es una ruta relativa o un ejecutable no hay nada que componer.
	}
	i = 1;
	pos = search_path(envp);
	if (pos == -1)
	{
		*err = ERROR_MALLOC;
		return(NULL);
	}
	enpath = ft_split(envp[pos], ':');//asegurarse de que no nos pasan una variable que contenga :
	if(enpath == NULL)
	{
		*err = ERROR_MALLOC;
		return(NULL);
	}
	while (enpath[i])
	{
		slash = ft_strjoin(enpath[i], "/");
		if (slash == NULL)
		{
			*err = ERROR_MALLOC;
			ft_free_double(&enpath);
			return (NULL);
		}
		path = ft_strjoin(slash, cmd);
		free(slash);
		if (path == NULL)
		{
			*err = ERROR_MALLOC;
			ft_free_double(&enpath);
			return (NULL);
		}
		if (access (path, F_OK ) == 0) //necesitamos distinguir errores "command not found" de "permission denied"
		{
			ft_free_double(&enpath);
			return (path);
		}
		i++;
		free(path);
	}
	ft_free_double(&enpath);
	*err = COM_NOT_FOUND;
	return (NULL);
}

e_errors create_child(t_task *task, char **envp, int in, int out)
{
    int pid;
    char *pathcmd;
    e_errors err;

    pid = fork();
    if (pid == -1)
        return (2);
    task->pid = pid;
    if (pid == 0)
    {
        if (out != STDOUT_FILENO)
        {
            dup2(out, STDOUT_FILENO);
            close(out);
        }
        if (in != STDIN_FILENO)
        {
            dup2(in, STDIN_FILENO); 
            close(in);
        }
        close_fds(3);
        err = apply_redirs(&(task->redir));
        if (err != 0)
        {
			perror("mini$hell");//revisar
			return(err);
		}
		pathcmd = com_path(task->cmd, envp, &err);
		if (err != 0)//////////////pasar a un handle error
		{
char *msg_error;
			if (err == COM_NOT_FOUND)
			{
				msg_error = ft_strjoin(task->cmd, ":Command not found\n");
				ft_putstr_fd(msg_error, 2);
				free(msg_error);
			}
			else
				perror("minishell");
			close_fds(0);
			return(err);
		}
		execve(pathcmd, task->argv, envp);
		err = errno;
		free(pathcmd);
    }
	if (out != STDOUT_FILENO)
		close(out);
	if (in != STDIN_FILENO)
		close(in);
    return (0);
}

e_errors exec_pipe(t_pipe *pipe_node, char **envp, int in)
{
    int pipefd[2];
    e_errors err;

    pipe(pipefd);

    if (pipe_node->left)
	{
        err = executor((t_tree *)pipe_node->left, envp, in, pipefd[1]);
        if(err != 0)
            return (err);
    }
    if (pipe_node->rigth)
	{
        err = executor(pipe_node->rigth, envp, pipefd[0], 1);
        if(err != 0)
            return (err);
    }
    return (0);
}

e_errors executor(t_tree *node, char **envp, int in, int out)
{
	t_pipe *pipe_node;
	t_task *task;
	e_errors	err;

    if (!node)
        return (0);
    
    if (node->type == PIPE)
    {
        pipe_node = (t_pipe *)node;
		err = exec_pipe(pipe_node, envp, in);
		if (err != 0)
			return (err);
	}
    else if (node->type == TASK)
    {
        task = (t_task *)node;
		err = create_child(task, envp, in, out);
		if (err != 0)
			return (err);
    }
    return (0);
}
