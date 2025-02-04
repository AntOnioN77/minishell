#include <stdlib.h>
/**
 * Libera la memoria reservada en matrices (ej: split)
 */
int	ft_free_double(char **s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		free (s[i]);
		i++;
	}
	free(s);
	return (1);
}

/*
	Libera y nulifica pnt.
	Protege de una liberacion incorrecta si pnt ya esta nulificado
*/
void ft_free_null(void **pnt)
{
	if (pnt == NULL || *pnt == NULL)
		return;
	free(*pnt);
	*pnt = NULL;
	return;
}

