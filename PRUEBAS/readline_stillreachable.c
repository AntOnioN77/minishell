#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/headers/libft.h"

int main() {
    char *input;

    while ((input = readline("read$ell>")) != NULL) 
	{
		if (ft_strnstr(input, "exit", 4))
			break;
        printf("You entered: %s\n", input);
        free(input);
    }

	free(input);
    // Limpia la memoria usada por readline
    rl_clear_history();

    return 0;
}
//==14491==     in use at exit: 228,787 bytes in 233 blocks
//==14491==   total heap usage: 528 allocs, 295 frees, 260,156 bytes allocated
