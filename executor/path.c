#include "minishell.h"
#include "executor.h"
#include <limits.h>
#include <sys/stat.h>
#include <stdio.h>

static char	*form_path(char *cmd, char **enpath, e_errors *err, char **slash)
{
	char	*pathcmd;

	if (*slash == NULL)
	{
		*err = ERROR_MALLOC;
		free_null_arr(&enpath);
		return (NULL);
	}
	pathcmd = ft_strjoin(*slash, cmd);
	free(*slash);
	if (pathcmd == NULL)
	{
		*err = ERROR_MALLOC;
		free_null_arr(&enpath);
		return (NULL);
	}
	return (pathcmd);
}

static char	*match_pathcmd(char *cmd, char **enpath, e_errors *err)
{
	char	*pathcmd;
	char	*slash;
	int		i;

	i = 0;
	while (enpath[++i])
	{
		slash = ft_strjoin(enpath[i], "/");
		pathcmd = form_path(cmd, enpath, err, &slash);
		if (pathcmd == NULL)
			return (NULL);
		if (access(pathcmd, F_OK) == 0)
		{
			free_null_arr(&enpath);
			if (access(pathcmd, X_OK) == 0)
				return (pathcmd);
			*err = NO_PERMISSION;
			free(pathcmd);
			return (NULL);
		}
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
	int		pos;
	char	*subpath;

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

static int	is_absolutepath(char *cmd, e_errors *err)
{
	struct stat	tipe;

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

char	*com_path(char *cmd, char **envp, e_errors *err)
{
	char	**enpath;
	int		pos;

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
