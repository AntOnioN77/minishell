#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_env_modification(char *envp[]) {
    // 1. Intentar modificar el puntero original
    char *new_env[] = {"NEW_VAR=test", NULL};
    envp = new_env;
    printf("1. Modificación del puntero original: %s\n", envp[0]);
/*
    // 2. Intentar modificar el contenido del string
    if (envp[0] != NULL) {
        envp[0][0] = 'M';  // Intenta modificar el primer carácter
        printf("2. Modificación del string: %s\n", envp[0]);
    }
*/
    // 3. Intentar asignar nuevo string
    envp[0] = "MODIFIED=value";
    printf("3. Asignación de nuevo string: %s\n", envp[0]);
}

int main(int argc, char *argv[], char *envp[]) {
    printf("Variables de entorno originales:\n");
    for (int i = 0; envp[i] != NULL; i++) {
        printf("%s\n", envp[i]);
    }
    
    printf("\nPruebas de modificación:\n");
    test_env_modification(envp);
    
    // Test getenv después de las modificaciones
    printf("\nTest getenv():\n");
    char *test_var = getenv("MODIFIED");
    printf("getenv(\"NEW_VAR\") = %s\n", test_var);
    
    return 0;
}