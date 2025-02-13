#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *custom_realloc(void **pnt, size_t oldsize, size_t newsize)
{
	void *newpnt;

	if (*pnt == NULL)
		return (NULL);
	if(oldsize >= newsize)
		return (*pnt);
	newpnt = calloc(1, newsize);
	if(newpnt == NULL)
		return(NULL);
	memcpy(newpnt, *pnt, oldsize);
	free(*pnt);
	*pnt = newpnt;
	return (newpnt);
}

int main() {
    char *buffer = calloc(1, 10); // Reservamos 10 bytes
    if (!buffer) {
        printf("Error al asignar memoria inicial\n");
        return 1;
    }

    strcpy(buffer, "Hola"); // Guardamos "Hola"
    printf("Antes de realloc: %s (tamaño: 10)\n", buffer);

    custom_realloc((void **)&buffer, 10, 20); // Expandimos a 20 bytes

    if (!buffer) {
        printf("Error al reasignar memoria\n");
        return 1;
    }

    printf("Después de realloc: %s (tamaño: 20)\n", buffer);

    // Modificamos el contenido para ver si se mantiene
    strcat(buffer, " Mundo");
    printf("Después de concatenar: %s\n", buffer);

    free(buffer); // Liberamos la memoria
    return 0;
}
