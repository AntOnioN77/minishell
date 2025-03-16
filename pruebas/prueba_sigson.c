#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void handler_child(int signal)
{
    (void)signal;
    printf("HANDLERR_CHILD proceso(%d)", getpid());

}

/**
 * Crea un proceso hijo que ejecuta el comando "sleep 4"
 * usando fork() y execve()
 */
void ejecutar_sleep() {
    pid_t pid;
    int status;
    
    // Argumentos para el comando sleep
    char *args[] = {"sleep", "4", NULL};
    
    // Ruta absoluta al comando sleep
    char *comando = "/bin/sleep";
    
    // Variables de entorno (vacío en este caso)
    char *env[] = {NULL};
signal(SIGINT, SIG_IGN);
    // Crear proceso hijo
    pid = fork();
    
    if (pid < 0) {
        // Error al crear el proceso hijo
        perror("Error en fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Código ejecutado por el proceso hijo

signal(SIGINT, handler_child);
        printf("Proceso hijo (PID: %d) iniciando execve con sleep 4\n", getpid());
        


        // Ejecutar el comando sleep con execve
        if (execve(comando, args, env) == -1) {
            perror("Error en execve");
            exit(EXIT_FAILURE);
        }

        while(1)
        {
            sleep(1);
            printf("soy el hijo\n");
        }
        // Este código nunca se ejecuta si execve tiene éxito
        printf("Este mensaje nunca debería imprimirse\n");
    } else {
        // Código ejecutado por el proceso padre
        printf("Proceso padre (PID: %d) esperando a que el hijo termine\n", getpid());
        
        // Esperar a que el proceso hijo termine
        if (waitpid(pid, &status, 0) == -1) {
            perror("Error en waitpid");
            exit(EXIT_FAILURE);
        }
        
        if (WIFEXITED(status)) {
            printf("Proceso hijo terminó con código de salida: %d\n", WEXITSTATUS(status));
        }
    }
}

// Ejemplo de uso
int main() {
    printf("Iniciando programa principal\n");
    ejecutar_sleep();
    printf("Programa principal terminado\n");
    return 0;
}