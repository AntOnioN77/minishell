#include "../libft/headers/libft.h"
#include "../minishell.h"

void unquote(char *str) {
    if (!str || !*str) return;

    int len = strlen(str);
    if (len < 2) return;  // Si la longitud es menor a 2, no puede haber comillas para desenvolver

    char quote_type = 0;
    int read_pos = 0;
    int write_pos = 0;

    // Verificar si hay comillas coincidentes al inicio y final
    if ((str[0] == '"' || str[0] == '\'') && str[len-1] == str[0]) {
        // Verificar que no haya una comilla sin cerrar del mismo tipo entre medio
        int quote_count = 0;
        for (int i = 1; i < len - 1; i++) {
            if (str[i] == str[0]) {
                quote_count++;
            }
        }
        // Si el número de comillas intermedias es par, entonces tenemos un par válido
        if (quote_count % 2 == 0) {
            quote_type = str[0];
            read_pos = 1;
            len--;  // Ignorar la última comilla
        }
    }

    while (read_pos < len) {
        // Si encontramos una comilla
        if (str[read_pos] == '"' || str[read_pos] == '\'') {
            // Si no es del tipo que estamos desenvolviendo, o si no estamos desenvolviendo,
            // la preservamos
            if (!quote_type || str[read_pos] != quote_type) {
                str[write_pos++] = str[read_pos];
            }
        }
        // Cualquier otro carácter se preserva
        else {
            str[write_pos++] = str[read_pos];
        }
        read_pos++;
    }

    // Terminar la cadena
    str[write_pos] = '\0';
}


#include <stdio.h>
int main(void)
{
	
	char *str= strdup("'");
	//printf("%s\n",str+33);
	str= str;
	unquote(str);
	printf("%s\n",str);
	//printf("%s\n", DELIMITERS);
	
	return(0);
}