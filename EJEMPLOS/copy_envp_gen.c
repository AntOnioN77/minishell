#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv, char **envp) {
    int i = 0;

    if (fork() == 0) {
        char **temp_envp;
        int envp_count = 0;

        // Contar el número de elementos en envp
        while (envp[envp_count]) {
            envp_count++;
        }

        // Asignar memoria para temp_envp
        temp_envp = malloc((envp_count + 2) * sizeof(char *));
        if (temp_envp == NULL) {
            perror("malloc");
            exit(1);
        }

        // Copiar envp a temp_envp
        for (i = 0; i < envp_count; i++) {
            temp_envp[i] = envp[i];
        }

        // Añadir la nueva variable de entorno
        temp_envp[i] = "NEW_VAR=constante de prueba";
        temp_envp[i + 1] = NULL;

        // Asignar temp_envp a envp
        envp = temp_envp;

        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>child:\n");
        for (i = 0; envp[i]; i++) {
            printf("%s\n", envp[i]);
        }

        // Liberar la memoria asignada
        free(temp_envp);

        exit(0);
    } else {
        wait(NULL);
    }

    return 0;
}