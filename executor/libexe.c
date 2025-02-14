#include <stdlib.h>


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


/**
 * Libera la memoria reservada en matrices (ej: split)
 */
int	free_null_arr(char ***s)
{
	int    i;
	char   **ptr;

	i = 0;
	if (!s || !*s || !**s)
		return (0);
	ptr = *s;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
	*s = NULL;  // Nullificamos el puntero original
	return (1);
}
