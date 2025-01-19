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
/**
 * 
 */
int create_pipe(/*t_tree *tree*/)
{
	int		fdp[2];
	char	*line;
	int controlbucle = 0;

	if (pipe (fdp) != 0)
	return (error_exe(1, NULL));
	line = NULL;
	while (!line && controlbucle < 4)
	{
		line = readline(">");
		if (line && line[0] == '\0')
			line = NULL;
		controlbucle++;
	}
	return (0);
}

/**
 * Se irán dividiendo las secciones que llegan a la función dependiendo del tipo
 * Si es un comando (TASK), se creará un hijo que lo ejecute
 * Si es una tubería (PIPE), creará una tubería que conecte diferentes partes
 * 		La tubería siempre tendrá en la parte izquierda un comando
 */
int execute(t_tree *tree, char **envp)
{
	char *path;
	char **cmd;

	if(tree->type == TASK)
    {
		cmd = ((t_task *)tree)->argv;
		if (cmd[0])
		{
			path = com_path(((t_task *)tree)->cmd, envp);
			if (!path)
				printf("command not found: %s\n", cmd[0]);
			else
			{
				create_child(/*tree, */envp, path, cmd);
				free(path);
			}
		}

    }
	else if (tree->type == PIPE)
	{
		create_pipe(/*tree*/);
	}
	return(0);
}

