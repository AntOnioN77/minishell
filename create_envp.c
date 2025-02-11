#include "minishell.h"
#include "libft/headers/libft.h"
#include <errno.h>
#include <linux/limits.h>

int count_to_null(void **pnt)
{
    int i = 0;
    while (pnt[i] != NULL)
	{
        i++;
    }
    return (i);
}

e_errors copy_prev_envp(char **original, t_environ *environ)
{
	int i;
	char *var;

	i = 0;
	while (original[i] != NULL)
	{
		var = ft_strdup(original[i]);
		environ->envp[i] = var;
		i++;
	}
	environ->next = i;
}
add_var(char *key, char *value, char **envp)
{

	//implementar
	//realoca nueva memoria si es necesario
}
//SHLVL PWD HOME OLDPWD $?
e_errors init_envp(t_environ *environ) 
{
	e_errors error;

	error = 0;
	char *path[PATH_MAX];

	if (environ->envp == NULL)
		return(errno);

	if (ft_getenv("SHLVL", environ) == NULL)
		error = add_var("SHLVL", "1", environ);
	
	if (ft_getenv("PWD", environ) == NULL)
	{
		if (getcwd(path, PATH_MAX) != NULL)
			error = add_var("PWD", path, environ);
		else
			return (errno);
	}
	
	if (ft_getenv("HOME", environ) == NULL)
		error = add_var("HOME", path, environ); //si no tenemos acceso a las variables de entorno, establece la carpeta actual como home (pues es la unica de la que tiene certeza existe y es utilizable)
	
	if (ft_getenv("OLDPWD", environ) == NULL)
		error = add_var("OLDPWD", "", environ); //al ser el principio de la ejecucion no hay una carpeta previa
	else
		error = change_var("OLDPWD", "", environ);// si existe, solo la reinicia como cadena vacía

	if (ft_getenv("?", environ) == NULL)
		error = add_var("?", "0", environ);
	
	return(error);
}

e_errors create_envp(char **original, t_environ *environ)
{
	int count;
	int error;

	
	if (original == NULL || *original == NULL)
	{
		environ->envp = ft_calloc(6, sizeof(char *));
		error = init_envp(environ);
	}
		else
	{
		count = count_to_null(original);
		environ->envp = ft_calloc(count * 2, sizeof(char *));// alocamos el doble de memoria necesaria, para no tener que realocar todo cada vez que creemos una nueva variable
		if (environ->envp = NULL)
			return(errno);
		environ->alloced = count * 2;
		error = copy_prev_envp(original, environ);
	}
	error = init_envp(environ);
	return (error);
}