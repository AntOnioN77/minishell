#include "../minishell.h"
#include "../libft/headers/libft.h"
#include <errno.h>
// #include <linux/limits.h>
#include <limits.h>

//mas adelante, descomponer este archivo en varios, en la carpeta environment



//a la libft?
int	count_to_null(void **pnt)
{
    int	i;

	i = 0;
    while (pnt[i] != NULL)
        i++;
    return (i);
}

e_errors	copy_prev_envp(char **original, t_environ *environ)
{
	int		i;
	char	*var;

	i = 0;
	while (original[i] != NULL)
	{
		var = ft_strdup(original[i]);
		if(var == NULL)
			return (errno);
		environ->envp[i] = var;
		i++;
	}
	environ->next = i;
	return (0);
}

e_errors	create_envp(char **original, t_environ *environ)
{
	int	count;
	int	error;

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
