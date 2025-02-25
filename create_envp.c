#include "minishell.h"
#include "libft/headers/libft.h"
#include <errno.h>
#include <linux/limits.h>

//mas adelante, descomponer este archivo en varios, en la carpeta environment

int	search_var(char **envp, const char* var)
{
	int pos;

//fprintf(stderr, "search_var var:%s --- ", var);
	if(!envp)
		return (-1);
	pos = 0;
	while (envp[pos])
	{
		if (ft_strncmp(envp[pos], var, ft_strlen(var)) == 0)
		{

			return (pos);
		}
		pos++;
	}
	return (-1);
}

char *ft_getenv(const char *name, char *envp[])
{
	int pos;
	char *word_start;

	pos = search_var(envp, name);
//fprintf(stderr, "search_var ret:%d\n", pos);
	if (pos == -1)
		return(NULL);
	word_start = ft_strchr(envp[pos], '=') + 1;
	return(word_start);
}

//a la libft?
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
//fprintf(stderr,"i:%d\n", i );
		var = ft_strdup(original[i]);
		if(var == NULL)
			return (errno);
		environ->envp[i] = var;
		i++;
	}
	environ->next = i;
	return (0);
}

//a la libft?
//newsize debe ser siempre superior a oldsize
void *custom_realloc(void **pnt, size_t oldsize, size_t newsize)
{
	void *newpnt;

	if (*pnt == NULL)
		return (NULL);
	if(oldsize >= newsize)
		return (*pnt);
	newpnt = ft_calloc(1, newsize);
	if(newpnt == NULL)
		return(NULL);
	ft_memcpy(newpnt, *pnt, oldsize);
	free(*pnt);
	*pnt = newpnt;
	return (newpnt);
}

e_errors change_var(char *key, char *newvalue, t_environ *environ)
{
	size_t len;
	size_t keylen;
	char *newvar;
	int i;
	char **envp;

	if (!key || !newvalue || !environ)
		return (1);//poco expresivo
	envp = environ->envp;
	keylen= ft_strlen(key);
	len = keylen + ft_strlen(newvalue) + 2;// +1 caracter nulo final, +1 signo "=" clave=valor
	newvar = ft_calloc(len, sizeof(char));
	if (newvar == NULL)
		return (errno);
	ft_strlcpy(newvar, key, len);
	ft_strlcat(newvar, "=", len);
	ft_strlcat(newvar, newvalue, len);
	i = 0;
	while(i < environ->next)
	{
		if (ft_strncmp(envp[i], key, keylen) == 0)
		{
			free(envp[i]);
			envp[i] = newvar;
			return(0);
		}
		i++;
	}
	free(newvar);
	return(1);

}

e_errors add_var(char *key, char *value, t_environ *environ)
{
	size_t len;
	char *newvar;

	if (key == NULL || value == NULL || key[0] == '\0')
		return(1);//asignar un valor más significativo para este error

//fprintf(stderr,"--------environ->alloced:%d\n", environ->alloced);
//fprintf(stderr,"--------environ->next:%d\n",environ->next);
	if (environ->next >= (environ->alloced - 1))
	{
//fprintf(stderr,"---------------------------------------------------------entra en realloc\n");
		if(!custom_realloc((void **)&(environ->envp), environ->alloced * sizeof(char *), environ->alloced * 2 * sizeof(char *)))
			return(errno);
		environ->alloced = environ->alloced * 2;
	}
	len = ft_strlen(key) + ft_strlen(value) + 2;// +1 caracter nulo final, +1 signo "=" clave=valor
	newvar = ft_calloc(len, sizeof(char));
	if (newvar == NULL)
		return (errno);
	ft_strlcpy(newvar, key, len);
	ft_strlcat(newvar, "=", len);
	ft_strlcat(newvar, value, len);
	environ->envp[environ->next] = newvar;
	environ->next=environ->next + 1;
	environ->envp[environ->next] = NULL;
	return(0);
}

static e_errors add_or_change_var(char *key, char *newvalue, t_environ *environ)
{
	e_errors error;

	error = 0;
	if (ft_getenv(key, environ->envp) == NULL)
		error = add_var(key, newvalue, environ);
	else
		change_var(key, newvalue, environ);
	return (error);

}

e_errors init_shlvl(t_environ *environ)
{
	e_errors error;
	char *num;


	if (ft_getenv("SHLVL", environ->envp) == NULL)
		error = add_var("SHLVL", "1", environ);
	else
	{
		num = ft_itoa(ft_atoi(ft_getenv("SHLVL", environ->envp)) + 1); 
		error = change_var("SHLVL", num, environ);
		free(num);
	}
	return(error);
}

//SHLVL PWD HOME OLDPWD $?
e_errors init_envp(t_environ *environ) 
{
	e_errors error;
	char *pathshell;
	char path[PATH_MAX];

	error = 0;
	if (environ == NULL || environ->envp == NULL)
		return(errno);

	error = init_shlvl(environ);

	if (getcwd(path, PATH_MAX) == NULL)
		return (errno);
	pathshell =	ft_strjoin(path,"/minishell/");
	if(!error)
		error = add_or_change_var("SHELL", pathshell, environ);
	free(pathshell);
	if(!error)
		error = add_or_change_var("PWD", path, environ);
	if (!error && ft_getenv("HOME", environ->envp) == NULL)
		error = add_var("HOME", path, environ); //si no tenemos acceso a las variables de entorno, establece la carpeta actual como home (pues es la unica de la que tiene certeza existe y es utilizable)
	if(!error)
		error = add_or_change_var("OLDPWD", "", environ);
	if(!error)
		error = add_or_change_var("?", "0", environ);
	return(error);
}

e_errors create_envp(char **original, t_environ *environ)
{
	int count;
	int error;

	ft_bzero(environ, sizeof(t_environ));////?? rompe ctrl+d (si ha habido un comando antes)
	/*environ->alloced = 0;
	environ->envp = 0;
	environ->local = 0;
	environ->localloced = 0;
	environ->locnext = 0;
	environ->next = 0;
	*/
	if (original == NULL || *original == NULL)
	{
		environ->envp = ft_calloc(12, sizeof(char *));
		if (environ->envp == NULL)
			return(errno);
		environ->alloced = 12;
	}
	else
	{
		count = count_to_null((void **)original);
		environ->envp = ft_calloc(count * 2, sizeof(char *));// alocamos el doble de memoria necesaria, para no tener que realocar todo cada vez que creemos una nueva variable
		if (environ->envp == NULL)
			return(errno);
		environ->alloced = count * 2;
		error = copy_prev_envp(original, environ);
	}
	error = init_envp(environ);
	return (error);
}