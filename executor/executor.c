#include "minishell.h"
#include "executor.h"

int	search_path(char **envp)
{
	int pos = 0;
	char *subpath;

	while (envp[pos])
	{
		subpath = ft_substr(envp[pos], 0, 4);
		//printf("%s\n", envp[i]);
		if (ft_strcmp(subpath, "PATH") == 0)
		{
			free(subpath);
			return (pos);
		}
		free(subpath);
		//printf("%s\n%d\n", envp[pos], );
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
	int 	i = 1;

	pos = search_path(envp);
	enpath = ft_split(envp[pos], ':');
	//printf("el path: %s\n", envp[pos]);
	while (enpath[i])
	{
		slash = ft_strjoin(enpath[i], "/");
		path = ft_strjoin(slash, cmd);
		free(slash);
		if (access (path, F_OK | X_OK) == 0)
			return (path);
		i++;
		free(path);
	}
	return (NULL);
}

/*int	child(char **envp, char *path, char **cmd)
{
	*if (!can_read(s[1]))
		return (0);*
	execve(path, cmd, envp);
	free (path);
	ft_free_double(cmd);
	return (1);
}*/

int	create_child(/*t_tree *tree,*/ char **envp, char *path, char **cmd)
{
	pid_t	pid;
	int		stat_loc = 0;

	pid = fork();
	if (pid < 0)
		return (error_exe(2, NULL));
	if (pid == 0)
	{
		execve(path, cmd, envp);
		//child(cmd, path);
		//if (!child(cmd, path))
		//{
			//return (1);
		//}
	}
	if (pid != 0)
		waitpid(pid, &stat_loc, 0);
	return (0);
}

int execute(t_tree *tree, char **envp)
{
	char *path;
	char **cmd;

	if(tree->type == TASK)
    {
		cmd = ((t_task *)tree)->argv;
		if (cmd)
		{
			path = com_path(((t_task *)tree)->cmd, envp);
			if (!path)
			{
				printf("command not found: %s\n", cmd[0]);
				//return (1);
			}
			else
			{
				//("%s\n", path);
				create_child(/*tree, */envp, path, cmd);
				//execve(path, cmd, envp);
			}
		}
    }
	else if (tree->type == PIPE)
	{
		/*
		int		fdp[2];

		if (pipe (fdp) != 0)
		return (error_exe(1, NULL));

		*/
	}
	return(0);
}

