//BORRA ESTA FUNCION ANTES DE ENTREGAR
#include "libft/headers/libft.h"
#include <stdio.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_nodes
{
	NONE,
	LINE,//probablemente innecesario
	PIPE,
	REDIR,//probablemente innecesario
	TASK
}	e_nodes;

typedef enum e_symbols
{
	none,
	infile,//<
	outfile,//>
	heredoc,//<<
	append//>>
} e_symbols;

/*Este struct no tiene un caso de uso real, funciona como interface. Los nodos del arbol
son en realidad t_line t_pipe t_redir t_task.
El uso de una interface nos permite pasar cualquiera de estos tipos como argumento a una
función. El elemento type, permite a esa función determinar que tipo de dato ha recibido
en realidad.*/
typedef struct s_tree
{
	e_nodes type;
}	t_tree;

/*La linea recibida como task se parsea de izquuierda a derecha, sucesivas redirecciones
se sobreescriben. de manera que hay un solo t_redir por cada t_task.
si no se encuentra ninguna redireccion los e_symbols pemanecen "none"
*/
typedef struct s_redir
{
	e_nodes	type;

	e_symbols	insymbol;//< o <<
	char		*infoo;// será un archivo para <, O un separator para <<

	e_symbols	outsymbol;//>> o >
	char		*outfile;
}	t_redir;

typedef struct s_task
{
	e_nodes	type;

	t_redir redir;
	char	*cmd;
	char	**argv;
}	t_task;

//si left o rigth fuesen NULL indica error de reserva de memoria, hay que liverar el arbol entero y lanzar error.
//Un pipe tal que "ls|   (nada)" generaria: pipe->rigth->task, y task tendria sus elementos vacios lo cual no es un error, este ejemplo en concreto deja a bash pendiente de entrada.
//left siempre debe contener un elemento task, y no puede contener otra cosa
//rigth puede contener otro elemento pipe o un elemento task
typedef struct s_pipe {
	e_nodes	type;
	t_task	*left;
	t_tree	*rigth;
}	t_pipe;

//FUNCIONES PARA CONSTRUIR ARBOL SINTACTICO
t_tree *processline(char *line);
void print_tree(t_tree *node, int depth);
void	skip_redir(char **segment, char *end);
char	*ft_strnchr(const char *s, int c, int n);
void skip_quotes(char **strpnt, char *end);
int strnchr_outquot(char **str, char *end, char c);
t_task *createtask(char *segment, char *end);
t_tree *createpipe(char *line,char *pnt);
void	getpntword(char **segment, char *end, char **dst);
void	get_redir(char **segment, char *end, t_redir *redir);
int count_cmdflags(char *segment, char *end);
int parse_task(char *segment, char *end, t_task *task);
int parsepipe(char *line, t_tree **ret);


#define WHITESPACES " \r\n\v\t"
