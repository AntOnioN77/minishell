#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void) {
    char *input;

    while (1) {
        // Mostrar un prompt personalizado y leer la entrada
        input = readline("minishell> ");

        // Salir si la entrada es NULL (Ctrl+D) o si el usuario escribe "exit"
        if (!input || strcmp(input, "exit") == 0) {
            free(input);
            break;
        }

        // Si la entrada no está vacía, agregarla al historial
        if (*input)
            add_history(input);

        // Imprimir lo que el usuario escribió (puedes procesarlo después)
        printf("Comando: %s\n", input);

        // Liberar la memoria asignada por readline
        free(input);
    }

    // Limpiar el historial al salir
    rl_clear_history();
    return 0;
}