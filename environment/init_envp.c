#include "../minishell.h"
#include "../libft/headers/libft.h"
#include <errno.h>
// #include <linux/limits.h>
#include <limits.h>

e_errors	set_shlvl(t_environ *environ)
{
	e_errors	error;
	char		*num;

	if (ft_getenv("SHLVL", environ->envp) == NULL)
		error = add_var("SHLVL", "1", environ);
	else
	{
		num = ft_itoa(ft_atoi(ft_getenv("SHLVL", environ->envp)) + 1);
		error = change_var("SHLVL", num, environ);
		free(num);
	}
	return (error);
}

e_errors	set_pathshell(t_environ *environ, char *path, e_errors error)
{
	char		*pathshell;

	pathshell = ft_strjoin(path, "/minishell");
	if (!error && ft_getenv("SHELL", environ->envp) == NULL)
		error = add_var("SHELL", pathshell, environ);
	else if (!error)
		error = change_var("SHELL", pathshell, environ);
	free(pathshell);
	return (error);
}

//SHLVL PWD HOME OLDPWD $?
e_errors	init_envp(t_environ *environ)
{
	e_errors	error;
	char		path[PATH_MAX];

	if (environ == NULL || environ->envp == NULL)
		return (errno);
	error = set_shlvl(environ);
	if (getcwd(path, PATH_MAX) == NULL)
		return (errno);
	error = set_pathshell(environ, path, error);
	if (!error && (ft_getenv("PWD", environ->envp) == NULL))
		error = add_var("PWD", path, environ);
	else if (!error)
		error = change_var("PWD", path, environ);
	if (!error && ft_getenv("HOME", environ->envp) == NULL)
		error = add_var("HOME", path, environ); //si no tenemos acceso a las variables de entorno, establece la carpeta actual como home (pues es la unica de la que tiene certeza existe y es utilizable)
	if (!error && ft_getenv("OLDPWD", environ->envp) == NULL)
		error = add_var("OLDPWD", "", environ); //al ser el principio de la ejecucion no hay una carpeta previa
	else if (!error)
		error = change_var("OLDPWD", "", environ);// si existe, solo la reinicia como cadena vacía
	if (!error && ft_getenv("?", environ->envp) == NULL)
		error = add_var("?", "0", environ);
	else if (!error)
		error = change_var("?", "0", environ);
	return (error);
}
