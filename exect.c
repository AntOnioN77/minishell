#include "minishell.h"


char	*com_path(char *cmd, char **envp)
{
	char	*path;
	char	*slash;
	char	**enpath;
	int i = 1;

	//Se puede sacar también con ft_substr
	enpath = ft_split(envp[22], ':');
	//printf("%s\n", envp[22]);
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


int execute(t_tree *tree, char **envp)
{
	char *path;
	char **cmd;
	int i = 17;

	if(tree->type == TASK)
    {
		path = com_path(((t_task *)tree)->cmd, envp);
		cmd = ((t_task *)tree)->argv;
		//printf("%s\n", path);
		execve(path, cmd, envp);
    }
	return(0);
}