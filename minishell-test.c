// gcc -g3 minishell-test.c minishell.c -L. -lft -lreadline

#include "minishell.h"
#include <assert.h>

void test_minishell(void)
{
    t_tree *tree;
    char *tests[] = {
		"<input",									//redir solo
		"ls",   									// 1. Comando simple sin argumentos
        "ls -la",                                   // 1. Comando simple con argumentos
		"ls -l | grep .c",                          // 2. Pipe simple
        "ls -l | grep .c | wc -l",                  // 3. Múltiples pipes
        "cat < input.txt",                          // 4. Redirección de entrada
        "echo hola > output.txt",                   // 5. Redirección de salida
        "echo 'texto con espacios'",                // 6. Comillas simples
        "echo \"texto con espacios\"",              // 7. Comillas dobles
        "ls    -l     -a",                          // 8. Múltiples espacios
        "ls -l > output.txt | wc -l",               // 9. Combinación de pipe y redirección
        "cat << EOF",                               // 10. Heredoc
		"cat <input.txt", 							// Redirección de entrada sin espacio
		"cat<input.txt",							// Redirección de entrada sin espacio y con el comando sin espacio
		"ls -la <infile",                           // Comando simple con argumentos y redir in
        "ls -la <infile <infile2",                  // Comando simple con argumentos y dos infiles, infile esperado: infile2
        NULL
    };

    printf("\n=== Iniciando tests de Minishell ===\n\n");
    
    for (int i = 0; tests[i] != NULL; i++) {
        printf("\nTest %d: '%s'\n", i + 1, tests[i]);
        printf("-----------------------------------------------\n");
        
        tree = processline(strdup(tests[i]));  // Usamos strdup porque processline modifica la string
        if (tree == NULL) {
            printf("Error: processline devolvió NULL\n");
            continue;
        }
        
        print_tree(tree, 0);
        free_tree(tree);
        printf("\n");
    }

    printf("\n=== Tests completados ===\n");
}

int main(void)
{
    test_minishell();
    return 0;
}
