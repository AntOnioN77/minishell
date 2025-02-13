//compilacion:  cc errorprueba.c -L ../libft -lft
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "../libft/headers/libft.h"


int main() {
	char *num;
	for (int i = 0; i <= 200; i++) {
       errno = i;
       //write(1,"errno %d:", 8);
	   num = ft_itoa(i);
       perror(num);
		free(num);
   }
   return 0;
}