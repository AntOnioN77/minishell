#include "minishell.h"
#include "executor.h"
#include <limits.h>
#include <sys/stat.h>
#include <stdio.h>


static char *match_pathcmd(char *cmd, char **enpath, e_errors *err)
{
	char *pathcmd;
	char *slash;
	int i;
	
	i= 1;
	while (enpath[i])
	{
		slash = ft_strjoin(enpath[i], "/");
		if (slash == NULL)
		{
			*err = ERROR_MALLOC;
			free_null_arr(&enpath);
			return (NULL);
		}
		pathcmd = ft_strjoin(slash, cmd);
		free(slash);
		if (pathcmd == NULL)
		{
			*err = ERROR_MALLOC;
			free_null_arr(&enpath);
			return (NULL);
		}
		if (access(pathcmd, F_OK) == 0)
		{
			free_null_arr(&enpath);
			if (access(pathcmd, X_OK) == 0)
				return (pathcmd);
			*err = NO_PERMISSION;
			free(pathcmd);
			return (NULL);
		}
		i++;
		free(pathcmd);
	}
	free_null_arr(&enpath);
	*err = COM_NOT_FOUND;
	return (NULL);
}

/**
 * Busca el array que comienza con "PATH" (donde se encuentran las rutas de los
 * ejecutables) y devuelve su posición.
 */
int	get_index_path(char **envp)
{
	int pos;
	char *subpath;

	pos = 0;
	while (envp[pos])
	{
		subpath = ft_substr(envp[pos], 0, 5);
		if (!subpath)
			return (-1);
		if (ft_strncmp(subpath, "PATH=", ft_strlen(subpath)) == 0)
		{
			free(subpath);
			return (pos);
		}
		free(subpath);
		pos++;
	}
	return (-1);
}

static int is_absolutepath(char *cmd, e_errors *err)
{
	struct stat tipe;
	if (access(cmd, F_OK) == 0)
	{
		stat(cmd, &tipe);
		if ((tipe.st_mode & 0170000) == (0040000))
		{
			*err = IS_A_DIR;
			return (0);
		}
		if (access(cmd, X_OK))
		{
			*err = NO_PERMISSION;
			return (0);
		}
		return (1);
	}
	else
	{
		*err = NO_EXIST;
		return (0);
	}
}


char *com_path(char *cmd, char **envp, e_errors *err)
{
	char **enpath;
	int pos;

	*err = 0;
	if (cmd && ft_strchr(cmd, '/'))
	{
		if (is_absolutepath(cmd, err))
			return (cmd);
		return (NULL);
	}
	pos = get_index_path(envp);
	if (pos == -1)
	{
		*err = NO_EXIST;
		return (NULL);
	}
	enpath = ft_split(envp[pos], ':');
	if (enpath == NULL)
	{
		*err = ERROR_MALLOC;
		return (NULL);
	}
	return (match_pathcmd(cmd, enpath, err));
}

static e_errors child_error_handler(e_errors err, char *cmd)
{

	if (err != 0)
	{
		if (cmd == NULL)
			return(child_error_handler_fail);
		if (err == COM_NOT_FOUND)
			print_error(cmd, ": Command not found\n");
		else if (err == IS_A_DIR)
			print_error(cmd, ": Is a directory\n");
		else if (err == NO_PERMISSION)
		{
			err = 126;
			print_error(cmd, ": Permission denied\n");
		}
		else if (err == NO_EXIST)
		{
			err = 127;
			print_error(cmd, ": No such file or directory\n");
		}
		else
			perror("minishell");
		close_fds(0);
		return (err);
	}
	return (0);
}

static e_errors repipe_child(t_task *task, int in, int out, char **word_fail)
{
	e_errors err;
//fprintf(stderr,"process:%d line:158 executor.c repipechild in:%d  out%d\n", getpid(),in, out);
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
	close(3);
	close(4);//soluciona lo de el cat|cat|ls pero no es pretty
	err = apply_redirs(&(task->redir), word_fail);
//fprintf(stderr, "!!!!!!!!!!!!!!!! executor.c 168¬ err:%d\n", (int)err);
	if (err != 0)
	{
		return (err);
	}
//test_fds("176 executor.c repipechild");
	return (0);
}

int is_builtin(char *cmd)
{
	if(!ft_strcmp(cmd, "echo") ||!ft_strcmp(cmd, "cd") ||!ft_strcmp(cmd, "pwd")
		||!ft_strcmp(cmd, "export") ||!ft_strcmp(cmd, "unset") ||!ft_strcmp(cmd, "env")
		||!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}



e_errors builtins_exe(t_task *task, t_environ *environ)
{
	char *cmd;

	(void)environ;//silenciar unused eviron

	cmd = task->cmd;
	if (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "export")
	|| !ft_strcmp(cmd, "unset"))
	{
		return (FINISH);
	}

	if (!ft_strcmp(cmd, "echo"))
	{
		return (ft_echo(task));
	}
	if (!ft_strcmp(cmd, "pwd"))
	{
		ft_putstr_fd(ft_getenv("PWD", environ->envp), 1);
		ft_putchar_fd('\n', 1);
	}
	if (!ft_strcmp(cmd, "env"))
	{
		ft_env(environ);
	}

	return (FINISH);
}


e_errors create_child(t_task *task, t_environ *environ , int in, int out)
{
	int pid;
	char *pathcmd;
	e_errors err;
	char *word_fail;
	char **envp;


	envp = environ->envp;
	word_fail = NULL;
	err = 0;
	pid = fork();
	if (pid == -1)
		return (errno);
	task->pid = pid;
	if (pid == 0)
	{
//printf("Proceso hijo: PID=%d, PPID=%d\n", getpid(), getppid());
		err = repipe_child(task, in, out, &word_fail);
		if(child_error_handler(err, word_fail))
			return (1);
		if(is_builtin(task->cmd))
			return(builtins_exe(task, environ));
		pathcmd = com_path(task->cmd, envp, &err); 			//no Builtin
		if (err)
			return (child_error_handler(err, task->cmd));
		rl_clear_history();
		execve(pathcmd, task->argv, envp);
		err = errno;
		free(pathcmd);
	}
	if (out != STDOUT_FILENO)
		close(out);
	if (in != STDIN_FILENO)
		close(in);
	return (err);
}

e_errors exec_pipe(t_pipe *pipe_node, t_environ *environ, int in)
{
    int pipefd[2];
    e_errors err;

    pipe(pipefd);

    if (pipe_node->left)
	{
        err = executor((t_tree *)pipe_node->left, environ, in, pipefd[1]);
        if(err != 0)
            return (err);
    }
    if (pipe_node->rigth)
	{
        err = executor(pipe_node->rigth, environ, pipefd[0], STDOUT_FILENO);
        if(err != 0)
            return (err);
    }
    return (0);
}

e_errors executor(t_tree *node, t_environ *environ , int in, int out)
{
	t_pipe *pipe_node;
	t_task *task;
	e_errors	err;


    if (!node)
        return (0);
    
    if (node->type == PIPE)
    {
        pipe_node = (t_pipe *)node;
		err = exec_pipe(pipe_node, environ, in);
		if (err != 0)
			return (err);
	}
    else if (node->type == TASK)
    {
        task = (t_task *)node;
		err = create_child(task, environ, in, out);
//fprintf(stderr, "executor.c 247¬ variable error:%d\n", (int)err);
		if (err != 0)
			return (err);
    }
    return (0);
}
