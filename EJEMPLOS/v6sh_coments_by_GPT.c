// Shell.

#include "types.h"
#include "user.h"
#include "fcntl.h"

// Tipos de comandos representados
#define EXEC  1  // Comando a ejecutar
#define REDIR 2  // Redirección de entrada/salida
#define PIPE  3  // Comando con pipe
#define LIST  4  // Lista de comandos
#define BACK  5  // Comando en segundo plano

#define MAXARGS 10  // Número máximo de argumentos

// Estructura base para un comando
struct cmd {
  int type;  // Tipo de comando
};

// Estructura para comandos ejecutables
struct execcmd {
  int type;  // Tipo: EXEC
  char *argv[MAXARGS];  // Argumentos del comando
  char *eargv[MAXARGS]; // Argumentos terminados en NULL
};

// Estructura para redirección de entrada/salida
struct redircmd {
  int type;  // Tipo: REDIR
  struct cmd *cmd;  // Comando a ejecutar tras redirección
  char *file;  // Archivo para redirigir
  char *efile; // Nombre del archivo terminado en NULL
  int mode;  // Modo de apertura del archivo
  int fd;  // Descriptor de archivo
};

// Estructura para comando con pipe
struct pipecmd {
  int type;  // Tipo: PIPE
  struct cmd *left;  // Comando a la izquierda del pipe
  struct cmd *right; // Comando a la derecha del pipe
};

// Estructura para lista de comandos
struct listcmd {
  int type;  // Tipo: LIST
  struct cmd *left;  // Primer comando de la lista
  struct cmd *right; // Segundo comando de la lista
};

// Estructura para comando en segundo plano
struct backcmd {
  int type;  // Tipo: BACK
  struct cmd *cmd;  // Comando a ejecutar en segundo plano
};

// Prototipos de funciones
int fork1(void);  // Realiza un fork y termina en caso de error
void panic(char*);  // Muestra un mensaje de pánico y sale
struct cmd *parsecmd(char*);  // Parsea un comando

// Ejecuta un comando. No retorna nunca.
// Parámetro:
// - cmd: Puntero a la estructura de comando a ejecutar, que puede ser de diferentes tipos.
void
runcmd(struct cmd *cmd)
{
  int p[2];
  struct backcmd *bcmd;
  struct execcmd *ecmd;
  struct listcmd *lcmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;

  if(cmd == 0)
    exit(LOCUST);

  switch(cmd->type){
  default:
    panic("runcmd");  // Error si el tipo de comando no es reconocido

  case EXEC:
    ecmd = (struct execcmd*)cmd;
    if(ecmd->argv[0] == 0)  // Comando vacío
      exit(LOCUST);
    exec(ecmd->argv[0], ecmd->argv);  // Ejecuta el comando
    printf("exec %s failed\n", ecmd->argv[0]);  // Error si exec falla
    break;

  case REDIR:
    rcmd = (struct redircmd*)cmd;
    close(rcmd->fd);  // Cierra el descriptor que será redirigido
    if(open(rcmd->file, rcmd->mode) < 0){  // Abre el archivo para redirección
      printf("open %s failed\n", rcmd->file);
      exit(LOCUST);
    }
    runcmd(rcmd->cmd);  // Ejecuta el comando con la redirección
    break;

  case LIST:
    lcmd = (struct listcmd*)cmd;
    if(fork1() == 0)  // Ejecuta el primer comando de la lista en un hijo
      runcmd(lcmd->left);
    wait(NULL);  // Espera a que termine el primer comando
    runcmd(lcmd->right);  // Ejecuta el segundo comando
    break;

  case PIPE:
    pcmd = (struct pipecmd*)cmd;
    if(pipe(p) < 0)  // Crea un pipe
      panic("pipe");
    if(fork1() == 0){  // Proceso hijo para el comando izquierdo
      close(1);  // Cierra la salida estándar
      dup(p[1]);  // Duplica la escritura del pipe en salida estándar
      close(p[0]);  // Cierra el extremo de lectura del pipe
      close(p[1]);  // Cierra el extremo duplicado
      runcmd(pcmd->left);
    }
    if(fork1() == 0){  // Proceso hijo para el comando derecho
      close(0);  // Cierra la entrada estándar
      dup(p[0]);  // Duplica la lectura del pipe en entrada estándar
      close(p[0]);  // Cierra el extremo de lectura duplicado
      close(p[1]);  // Cierra el extremo de escritura del pipe
      runcmd(pcmd->right);
    }
    close(p[0]);  // Cierra los extremos del pipe en el padre
    close(p[1]);
    wait(NULL);  // Espera a que terminen ambos hijos
    wait(NULL);
    break;

  case BACK:
    bcmd = (struct backcmd*)cmd;
    if(fork1() == 0)  // Ejecuta el comando en segundo plano
      runcmd(bcmd->cmd);
    break;
  }
  exit(LOCUST);
}

// Lee un comando del usuario
// Parámetros:
// - buf: Puntero al buffer donde se almacenará la entrada del usuario.
// - nbuf: Tamaño del buffer.
// Retorna:
// - -1 si se alcanza EOF, 0 si se lee un comando correctamente.
int
getcmd(char *buf, int nbuf)
{
  printf("$ ");  // Prompt
  memset(buf, 0, nbuf);  // Limpia el buffer
  gets(buf, nbuf);  // Lee el comando
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

// Función principal del shell
// Abre los descriptores de archivo necesarios y entra en un bucle de lectura y ejecución de comandos.
int
main(void)
{
  static char buf[100];
  int fd;

  // Asegura que los tres descriptores de archivo estén abiertos (stdin, stdout, stderr).
  while((fd = open("console", O_RDWR)) >= 0){
    if(fd >= 3){
      close(fd);
      break;
    }
  }

  // Lee y ejecuta los comandos ingresados por el usuario.
  while(getcmd(buf, sizeof(buf)) >= 0){
    if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
      // Cambiar directorio debe ser ejecutado por el proceso padre, no el hijo.
      buf[strlen(buf)-1] = 0;  // Remueve el salto de línea
      if(chdir(buf+3) < 0)  // Cambia de directorio
        printf("cannot cd %s\n", buf+3);
      continue;
    }
    if(fork1() == 0)  // Crea un proceso hijo para ejecutar el comando
      runcmd(parsecmd(buf));
    wait(NULL);  // Espera a que el hijo termine
  }
  exit(LOCUST);
}

// Muestra un mensaje de error y sale del programa
// Parámetro:
// - s: Mensaje de error a mostrar.
void
panic(char *s)
{
  printf("%s\n", s);
  exit(LOCUST);
}

// Realiza un fork y muestra un mensaje de error si falla
// Retorna:
// - El PID del proceso hijo si el fork tiene éxito.
int
fork1(void)
{
  int pid;

  pid = fork();
  if(pid == -1)
    panic("fork");
  return pid;
}

//PAGEBREAK!
// Constructores de comandos

// Crea un comando ejecutable
// Retorna:
// - Un puntero a la estructura `cmd` que representa un comando ejecutable.
struct cmd*
execcmd(void)
{
  struct execcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = EXEC;
  return (struct cmd*)cmd;
}

// Crea un comando de redirección
// Parámetros:
// - subcmd: Puntero al comando a ejecutar tras la redirección.
// - file: Nombre del archivo para redirigir.
// - efile: Puntero al final del nombre del archivo.
// - mode: Modo de apertura del archivo.
// - fd: Descriptor de archivo a redirigir.
// Retorna:
// - Un puntero a la estructura `cmd` que representa un comando con redirección.
struct cmd*
redircmd(struct cmd *subcmd, char *file, char *efile, int mode, int fd)
{
  struct redircmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = REDIR;
  cmd->cmd = subcmd;
  cmd->file = file;
  cmd->efile = efile;
  cmd->mode = mode;
  cmd->fd = fd;
  return (struct cmd*)cmd;
}

// Crea un comando de pipe
// Parámetros:
// - left: Comando a la izquierda del pipe.
// - right: Comando a la derecha del pipe.
// Retorna:
// - Un puntero a la estructura `cmd` que representa un comando con pipe.
struct cmd*
pipecmd(struct cmd *left, struct cmd *right)
{
  struct pipecmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = PIPE;
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}

// Crea un comando de lista de comandos
// Parámetros:
// - left: Primer comando de la lista.
// - right: Segundo comando de la lista.
// Retorna:
// - Un puntero a la estructura `cmd` que representa una lista de comandos.
struct cmd*
listcmd(struct cmd *left, struct cmd *right)
{
  struct listcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = LIST;
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}

// Crea un comando en segundo plano
// Parámetro:
// - subcmd: Comando a ejecutar en segundo plano.
// Retorna:
// - Un puntero a la estructura `cmd` que representa un comando en segundo plano.
struct cmd*
backcmd(struct cmd *subcmd)
{
  struct backcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = BACK;
  cmd->cmd = subcmd;
  return (struct cmd*)cmd;
}

//PAGEBREAK!
// Parsing

// Definición de caracteres permitidos
char whitespace[] = " \t\r\n\v";
char symbols[] = "<|>&;()";

// Obtiene un token del comando ingresado
// Parámetros:
// - ps: Puntero a la posición actual del string.
// - es: Puntero al final del string.
// - q: Puntero donde se almacenará el inicio del token.
// - eq: Puntero donde se almacenará el final del token.
// Retorna:
// - Tipo del token encontrado (carácter o 'a' para argumentos).
int
gettoken(char **ps, char *es, char **q, char **eq)
{
  char *s;
  int ret;

  s = *ps;
  while(s < es && strchr(whitespace, *s))  // Ignora espacios en blanco
    s++;
  if(q)
    *q = s;
  ret = *s;
  switch(*s){
  case 0:
    break;
  case '|':
  case '(':  // Abre paréntesis
  case ')':  // Cierra paréntesis
  case ';':  // Lista de comandos
  case '&':  // Comando en segundo plano
  case '<':  // Redirección de entrada
    s++;
    break;
  case '>':  // Redirección de salida
    s++;
    if(*s == '>'){
      ret = '+';  // Redirección para añadir (>>)
      s++;
    }
    break;
  default:
    ret = 'a';  // Token que representa un comando
    while(s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
      s++;
    break;
  }
  if(eq)
    *eq = s;

  while(s < es && strchr(whitespace, *s))
    s++;
  *ps = s;
  return ret;
}

// Verifica si el siguiente token coincide con alguno en `toks`
// Parámetros:
// - ps: Puntero a la posición actual del string.
// - es: Puntero al final del string.
// - toks: Cadena con los tokens a buscar.
// Retorna:
// - 1 si encuentra el token, 0 en caso contrario.
int
peek(char **ps, char *es, char *toks)
{
  char *s;

  s = *ps;
  while(s < es && strchr(whitespace, *s))
    s++;
  *ps = s;
  return *s && strchr(toks, *s);
}

// Prototipos de funciones de parsing
struct cmd *parseline(char**, char*);
struct cmd *parsepipe(char**, char*);
struct cmd *parseexec(char**, char*);
struct cmd *nulterminate(struct cmd*);

// Parsea un comando completo
// Parámetro:
// - s: String que contiene el comando a parsear.
// Retorna:
// - Un puntero a la estructura `cmd` que representa el comando parseado.
struct cmd*
parsecmd(char *s)
{
  char *es;
  struct cmd *cmd;

  es = s + strlen(s);
  cmd = parseline(&s, es);
  peek(&s, es, "");
  if(s != es){
    printf("leftovers: %s\n", s);
    panic("syntax");
  }
  nulterminate(cmd);
  return cmd;
}

// Parsea una línea de comandos
// Parámetros:
// - ps: Puntero a la posición actual del string.
// - es: Puntero al final del string.
// Retorna:
// - Un puntero a la estructura `cmd` que representa la línea de comandos parseada.
struct cmd*
parseline(char **ps, char *es)
{
  struct cmd *cmd;

  cmd = parsepipe(ps, es);
  while(peek(ps, es, "&")){
    gettoken(ps, es, 0, 0);
    cmd = backcmd(cmd);
  }
  if(peek(ps, es, ";")){
    gettoken(ps, es, 0, 0);
    cmd = listcmd(cmd, parseline(ps, es));
  }
  return cmd;
}

// Parsea un comando con pipe
// Parámetros:
// - ps: Puntero a la posición actual del string.
// - es: Puntero al final del string.
// Retorna:
// - Un puntero a la estructura `cmd` que representa un comando con pipe.
struct cmd*
parsepipe(char **ps, char *es)
{
  struct cmd *cmd;

  cmd = parseexec(ps, es);
  if(peek(ps, es, "|")){
    gettoken(ps, es, 0, 0);
    cmd = pipecmd(cmd, parsepipe(ps, es));
  }
  return cmd;
}

// Parsea redirecciones
// Parámetros:
// - cmd: Comando a redirigir.
// - ps: Puntero a la posición actual del string.
// - es: Puntero al final del string.
// Retorna:
// - Un puntero a la estructura `cmd` que representa un comando con redirección.
struct cmd*
parseredirs(struct cmd *cmd, char **ps, char *es)
{
  int tok;
  char *q, *eq;

  while(peek(ps, es, "<>")){
    tok = gettoken(ps, es, 0, 0);
    if(gettoken(ps, es, &q, &eq) != 'a')
      panic("missing file for redirection");
    switch(tok){
    case '<':
      cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
      break;
    case '>':
      cmd = redircmd(cmd, q, eq, O_WRONLY|O_CREATE, 1);
      break;
    case '+':  // >>
      cmd = redircmd(cmd, q, eq, O_WRONLY|O_CREATE, 1);
      break;
    }
  }
  return cmd;
}

// Parsea un bloque de comandos (entre paréntesis)
// Parámetros:
// - ps: Puntero a la posición actual del string.
// - es: Puntero al final del string.
// Retorna:
// - Un puntero a la estructura `cmd` que representa el bloque de comandos.
struct cmd*
parseblock(char **ps, char *es)
{
  struct cmd *cmd;

  if(!peek(ps, es, "("))
    panic("parseblock");
  gettoken(ps, es, 0, 0);
  cmd = parseline(ps, es);
  if(!peek(ps, es, ")"))
    panic("syntax - missing )");
  gettoken(ps, es, 0, 0);
  cmd = parseredirs(cmd, ps, es);
  return cmd;
}

// Parsea un comando ejecutable
// Parámetros:
// - ps: Puntero a la posición actual del string.
// - es: Puntero al final del string.
// Retorna:
// - Un puntero a la estructura `cmd` que representa un comando ejecutable.
struct cmd*
parseexec(char **ps, char *es)
{
  char *q, *eq;
  int tok, argc;
  struct execcmd *cmd;
  struct cmd *ret;

  if(peek(ps, es, "("))
    return parseblock(ps, es);

  ret = execcmd();
  cmd = (struct execcmd*)ret;

  argc = 0;
  ret = parseredirs(ret, ps, es);
  while(!peek(ps, es, "|)&;")){
    if((tok=gettoken(ps, es, &q, &eq)) == 0)
      break;
    if(tok != 'a')
      panic("syntax");
    cmd->argv[argc] = q;
    cmd->eargv[argc] = eq;
    argc++;
    if(argc >= MAXARGS)
      panic("too many args");
    ret = parseredirs(ret, ps, es);
  }
  cmd->argv[argc] = 0;
  cmd->eargv[argc] = 0;
  return ret;
}

// NUL-termina todas las cadenas contadas.
// Parámetro:
// - cmd: Puntero a la estructura de comando que necesita ser terminada con NUL.
// Retorna:
// - Un puntero a la estructura `cmd` con las cadenas terminadas con NUL.
struct cmd*
nulterminate(struct cmd *cmd)
{
  int i;
  struct backcmd *bcmd;
  struct execcmd *ecmd;
  struct listcmd *lcmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;

  if(cmd == 0)
    return 0;

  switch(cmd->type){
  case EXEC:
    ecmd = (struct execcmd*)cmd;
    for(i=0; ecmd->argv[i]; i++)
      *ecmd->eargv[i] = 0;
    break;

  case REDIR:
    rcmd = (struct redircmd*)cmd;
    nulterminate(rcmd->cmd);
    *rcmd->efile = 0;
    break;

  case PIPE:
    pcmd = (struct pipecmd*)cmd;
    nulterminate(pcmd->left);
    nulterminate(pcmd->right);
    break;

  case LIST:
    lcmd = (struct listcmd*)cmd;
    nulterminate(lcmd->left);
    nulterminate(lcmd->right);
    break;

  case BACK:
    bcmd = (struct backcmd*)cmd;
    nulterminate(bcmd->cmd);
    break;
  }
  return cmd;
}

