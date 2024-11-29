// Estructuras de comando
struct cmd {
  int type;  // Tipo de comando (EXEC, REDIR, PIPE, LIST, BACK)
};

/**
 * @brief Ejecuta un comando
 * @param cmd Puntero a la estructura del comando a ejecutar
 * @desc Función principal que ejecuta los diferentes tipos de comandos.
 *       Nunca retorna, siempre termina con exit(LOCUST)
 */
void runcmd(struct cmd *cmd) {
    // ... implementation ...
}

/**
 * @brief Obtiene un comando del usuario
 * @param buf Buffer donde se almacenará el comando
 * @param nbuf Tamaño del buffer
 * @return -1 si es EOF, 0 en caso contrario
 * @desc Lee un comando de la entrada estándar y lo almacena en el buffer
 */
int getcmd(char *buf, int nbuf) {
    // ... implementation ...
}

/**
 * @brief Función principal del shell
 * @desc Inicializa los descriptores de archivo y ejecuta el loop principal
 *       del shell que lee y ejecuta comandos
 * @return No retorna, termina con exit(LOCUST)
 */
int main(void) {
    // ... implementation ...
}

/**
 * @brief Maneja errores fatales
 * @param s Mensaje de error a mostrar
 * @desc Imprime el mensaje de error y termina el programa
 */
void panic(char *s) {
    // ... implementation ...
}

/**
 * @brief Wrapper seguro para fork()
 * @return PID del proceso hijo o entra en pánico si falla
 * @desc Ejecuta fork() y verifica que no haya errores
 */
int fork1(void) {
    // ... implementation ...
}

/**
 * @brief Constructor para comando de ejecución
 * @return Puntero a una estructura cmd inicializada para EXEC
 * @desc Crea y inicializa una estructura para ejecutar un comando simple
 */
struct cmd* execcmd(void) {
    // ... implementation ...
}

/**
 * @brief Constructor para comando de redirección
 * @param subcmd Comando que será redirigido
 * @param file Nombre del archivo para redirección
 * @param efile Puntero al final del nombre del archivo
 * @param mode Modo de apertura del archivo
 * @param fd Descriptor de archivo a redirigir
 * @return Puntero a una estructura cmd inicializada para REDIR
 */
struct cmd* redircmd(struct cmd *subcmd, char *file, char *efile, int mode, int fd) {
    // ... implementation ...
}

/**
 * @brief Constructor para comando de pipe
 * @param left Comando del lado izquierdo del pipe
 * @param right Comando del lado derecho del pipe
 * @return Puntero a una estructura cmd inicializada para PIPE
 */
struct cmd* pipecmd(struct cmd *left, struct cmd *right) {
    // ... implementation ...
}

/**
 * @brief Constructor para lista de comandos
 * @param left Primer comando de la lista
 * @param right Segundo comando de la lista
 * @return Puntero a una estructura cmd inicializada para LIST
 */
struct cmd* listcmd(struct cmd *left, struct cmd *right) {
    // ... implementation ...
}

/**
 * @brief Constructor para comando en background
 * @param subcmd Comando a ejecutar en background
 * @return Puntero a una estructura cmd inicializada para BACK
 */
struct cmd* backcmd(struct cmd *subcmd) {
    // ... implementation ...
}

/**
 * @brief Obtiene el siguiente token del input
 * @param ps Puntero al puntero de la cadena de entrada
 * @param es Puntero al final de la cadena de entrada
 * @param q Puntero donde se guardará el inicio del token
 * @param eq Puntero donde se guardará el final del token
 * @return Tipo de token encontrado
 * @desc Analiza la entrada y extrae el siguiente token
 */
int gettoken(char **ps, char *es, char **q, char **eq) {
    // ... implementation ...
}

/**
 * @brief Verifica el siguiente token sin consumirlo
 * @param ps Puntero al puntero de la cadena de entrada
 * @param es Puntero al final de la cadena de entrada
 * @param toks Cadena con los tokens válidos a buscar
 * @return true si el siguiente token está en toks
 */
int peek(char **ps, char *es, char *toks) {
    // ... implementation ...
}

/**
 * @brief Parser principal de comandos
 * @param s Cadena de entrada con el comando
 * @return Estructura de comando parseada
 * @desc Punto de entrada principal para el parsing de comandos
 */
struct cmd* parsecmd(char *s) {
    // ... implementation ...
}

/**
 * @brief Parsea una línea de comandos
 * @param ps Puntero al puntero de la cadena de entrada
 * @param es Puntero al final de la cadena de entrada
 * @return Estructura de comando parseada
 * @desc Maneja el parsing de comandos separados por ; y &
 */
struct cmd* parseline(char **ps, char *es) {
    // ... implementation ...
}

/**
 * @brief Parsea un pipe
 * @param ps Puntero al puntero de la cadena de entrada
 * @param es Puntero al final de la cadena de entrada
 * @return Estructura de comando parseada
 * @desc Maneja el parsing de comandos conectados por |
 */
struct cmd* parsepipe(char **ps, char *es) {
    // ... implementation ...
}

/**
 * @brief Parsea redirecciones
 * @param cmd Comando base a redireccionar
 * @param ps Puntero al puntero de la cadena de entrada
 * @param es Puntero al final de la cadena de entrada
 * @return Estructura de comando con redirecciones aplicadas
 * @desc Procesa operadores < y > para redirección
 */
struct cmd* parseredirs(struct cmd *cmd, char **ps, char *es) {
    // ... implementation ...
}

/**
 * @brief Parsea un bloque entre paréntesis
 * @param ps Puntero al puntero de la cadena de entrada
 * @param es Puntero al final de la cadena de entrada
 * @return Estructura de comando parseada
 * @desc Maneja el parsing de comandos entre paréntesis ()
 */
struct cmd* parseblock(char **ps, char *es) {
    // ... implementation ...
}

/**
 * @brief Parsea un comando de ejecución
 * @param ps Puntero al puntero de la cadena de entrada
 * @param es Puntero al final de la cadena de entrada
 * @return Estructura de comando parseada
 * @desc Parsea un comando simple con sus argumentos
 */
struct cmd* parseexec(char **ps, char *es) {
    // ... implementation ...
}

/**
 * @brief Termina todas las cadenas con NUL
 * @param cmd Estructura de comando a procesar
 * @return La misma estructura de comando procesada
 * @desc Asegura que todas las cadenas en la estructura terminen en NUL
 */
struct cmd* nulterminate(struct cmd *cmd) {
    // ... implementation ...
}
