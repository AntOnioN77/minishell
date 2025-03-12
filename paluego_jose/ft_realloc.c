#include "libft/headers/libft.h"
/**
 * Si el puntero es NULL, devuelve la reasignación de memoria
 * Si el tamaño de reasignación es 0, se libera el puntero y se devuelve NULL
 * Se reserva el nuevo tamaño de memoria y se le asigna lo que tenía el anterior
 * puntero hasta rellenarlo o hasta que se acabe. Se libera el puntero y se
 * devuelve el nuevo
 */
void	*ft_realloc(void *ptr, size_t size, size_t resize)
{
	void	*reptr;

	if (ptr == NULL)
		return (malloc(resize));
	if (resize == 0)
	{
		free(ptr);
		return (NULL);
	}
	reptr = malloc(resize);
	if (!reptr)
		return (NULL);
	if (resize < size)
		size = resize;
	ft_memcmp(reptr, ptr, size);
	free(ptr);
	return (reptr);
}
