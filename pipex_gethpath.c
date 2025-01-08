#include "minishell.h"

static char	**get_paths(char **envp)
{
	int		i;
	char	**paths;

	if (envp == NULL)
		return (NULL);
	i = -1;
	paths = NULL;
	while (envp[++i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			paths = ft_split(&(envp[i][5]), ':');
			if (*paths == NULL || **paths == '\0')
				return (NULL);
			break ;
		}
	}
	return (paths);
}

/*Writes the first valid path+command for execution found into the
pathname buffer.
If no valid path is found, returns 1*/
static int	path_selector(char *pathname, char **paths, char *command)
{
	int	i;

	if (pathname == NULL || paths == NULL || command == NULL)
		return (127);
	i = -1;
	while (paths[++i] != NULL)
	{
		ft_strlcpy(pathname, paths[i], PATH_MAX);
		ft_strlcat(pathname, "/", PATH_MAX);
		ft_strlcat(pathname, command, PATH_MAX + NAME_MAX);
		if (access(pathname, X_OK) == 0)
			return (0);
	}
	return (127);
}

static int	find_path(char **env, char *command, char *pathname)
{
	char	**paths;
	int		path_return;

	if (command && (access(command, X_OK) == 0))
	{
		ft_strlcpy(pathname, command, PATH_MAX);
		return (0);
	}
	paths = get_paths(env);
	path_return = path_selector(pathname, paths, command);
	if (path_return == 0)
	{
		free_split(paths);
		return (0);
	}
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(command, 2);
	if (paths != NULL)
	{
		ft_putstr_fd(": command not found\n", 2);
		free_split(paths);
	}
	else
		ft_putstr_fd(": No such file or directory\n", 2);
	return (path_return);
}