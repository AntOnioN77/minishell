// gcc -g3 parse-test.c minishell.c -L. -lft -lreadline

#include "../minishell.h"
#include <assert.h>

void test_minishell(void)
{
    t_tree *tree;
    char *tests[] = {
        "ls -la",                                   // 1. Comando simple con argumentos
		"ls -l |grep .c",							// 2. Pipe simple
        "ls -l | grep .c | wc -l",                  // 3. Múltiples pipes
        "cat < input.txt",                          // 4. Redirección de entrada
       "echo hola > output.txt",                   // 5. Redirección de salida
        "echo 'texto con espacios'",                // 6. Comillas simples
        "echo \"texto con espacios\"",              // 7. Comillas dobles
        "ls    -l     -a",                          // 8. Múltiples espacios
        "ls -l > output.txt | wc -l",               // 9. Combinación de pipe y redirección
        "cat << EOF",                               // 10. Heredoc
		"cat <input.txt", 							// 11. Redirección de entrada sin espacio 
		"cat<input.txt",							// 12. Redirección de entrada sin espacio y con el comando sin espacio 
		"ls -la <infile",                           // 13. Comando simple con argumentos y redir in  
        "ls -la <infile<infile2",                   // 14. Comando simple con argumentos y dos infiles, infile esperado: infile2 
		"\"\"|<input",								// 15. comillas  dobles vacias
		"<input",									// 16. redir solo
		"ls",   									// 17. Comando simple sin argumentos
		"<input ls| wc -a",							// 18. redireccion antes que el comando 
		"echo 'comillas''dos veces'",				// 19. comillas dos veces sin espacio en medio
		"echo 'comillas''dos veces'>outfile",		// 20. comillas dos veces sin espacio en medio y luego redir
		"echo >outfile'comillas''dos veces'",		// 21. redir y luego comillas dos veces sin espacio en medio
		"echo 'comillas'>outfile'dos veces'",		// 22. comillas cerradas y luego redir y luego comillas cerradas otra vez
		"'|",										// 23.
		"|'",										// 24.
		"$VAR",
		"$VAR$VAR", 
		"prefix $VAR suffix",
		"'$VAR'", 
		"\"$VAR\"", 
		"\"$VAR'$VAR'\"",
		"$NONEXISTENT",
		"$EMPTY", 
		"$SPACE", 
		"\"$VAR'$VAR'$VAR\"'$VAR'", 
		"$VAR\t$VAR\n$VAR", 
		"$QUOTES", 
		"$LONG", 
		"$", 
		"text$", 
		"$$$$VAR", 
		"$VAR!@#$VAR", 
		"\"$VAR", 
		"$VAR<NAME", 
		"", 
        NULL
    };

    printf("\n=== Iniciando tests de Minishell ===\n       compara con: ./a.out | diff - parse-test.check\n");
    
    for (int i = 0; tests[i] != NULL; i++) {
        printf("\nTest %d: '%s'\n", i + 1, tests[i]);
        printf("-----------------------------------------------\n");
        
        tree = processline(strdup(tests[i]));  // Usamos strdup porque processline modifica la string
        if (tree == NULL) {
            printf("Error: processline devolvió NULL\n");
            continue;
        }
		char *envp[] = {NULL};  // No necesitamos envp real porque mockeamos ft_getenv
		if(expand_tree(tree, envp))
			perror("expandtree:");//esta gestion de error es muy mejorable
        
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
