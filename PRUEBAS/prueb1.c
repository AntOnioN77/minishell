#include "libft/headers/libft.h"
#include <stdio.h>

int main(void)
{
	char line[] = "Hola";
	if(line[ft_strlen(line)] == '\0')
		printf("ok");
	return 0;
}