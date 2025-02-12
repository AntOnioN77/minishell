#include "minishell.h"
#include "executor.h"
#include <limits.h>


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

int create_child(t_task *task, char **envp, int in, int out)
{
    int pid;
    char *pathcmd;
    int err;

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
        if (err == 0)
        {
            pathcmd = com_path(task->cmd, envp);
            if (pathcmd == NULL)
            {
                close_fds(0);
                return(ERROR_MALLOC);
            }

            execve(pathcmd, task->argv, envp);
            err = errno;
            free(pathcmd);
        }
		perror("create_child");//revisar
        return(FINISH);
    }
	if (out != STDOUT_FILENO)
		close(out);
	if (in != STDIN_FILENO)
		close(in);
    return (0);
}

int exec_pipe(t_pipe *pipe_node, char **envp, int in)
{
    int pipefd[2];
    int err;

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

int executor(t_tree *node, char **envp, int in, int out)
{
	t_pipe *pipe_node;
	t_task *task;
	int	err;

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
