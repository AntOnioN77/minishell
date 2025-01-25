

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/headers/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# define WHITESPACES " \r\n\v\t"
# define DELIMITERS "|<> \r\n\v\t"


/*****************************************************************************/
/* 								ENUMS										 */
/*****************************************************************************/

typedef enum e_nodes
{
	NONE,
	LINE,//probablemente innecesario
	PIPE,
	REDIR,//probablemente innecesario
	TASK,
	SYNTAX
}	e_nodes;

typedef enum e_symbols
{
	none,
	infile,//<
	outfile,//>
	heredoc,//<<
	append//>>
} e_symbols;


//no implementado
typedef enum e_errors
{
	TREE_OK,
    ERROR_MALLOC,
    INVALID_TYPE,
	TASK_IS_VOID,
	SYNTAX_ERROR,
	REDLINE_FAIL
} e_errors;

/*****************************************************************************/
/* 								STRUCTS										 */
/*****************************************************************************/

/*_________________________TREE_STRUCTS______________________________________*/

/*Este struct no tiene un caso de uso real, funciona como interface. Los nodos del arbol
son en realidad t_line t_pipe t_redir t_task.
El uso de una interface nos permite pasar cualquiera de estos tipos como argumento a una
función. El elemento type, permite a esa función determinar que tipo de dato ha recibido
en realidad.*/
typedef struct s_tree
{
	e_nodes type;
	char	*line;		//solo el primer nodo del arbol contendrá line!=NULL
	char	*line_extra;//Solo en caso de "ls|(vacio)" line_extra !=NULL
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

typedef struct s_garbage {
	void	**pointers;
	int		size;
	int		current;
}	t_garbage;

typedef struct s_task
{
	e_nodes		type;
	char	*line;		//solo el primer nodo del arbol contendrá line!=NULL
	char	*line_extra;//Solo en caso de "ls|(vacio)" line_extra !=NULL
	t_redir 	redir;
	char		*cmd;
	char		**argv;
	t_garbage	garb;
	int   pid; 
}	t_task;

//si left o rigth fuesen NULL indica error de reserva de memoria, hay que liverar el arbol entero y lanzar error.
//Un pipe tal que "ls|   (nada)" generaria: pipe->rigth->task, y task tendria sus elementos vacios lo cual no es un error, este ejemplo en concreto deja a bash pendiente de entrada.
//left siempre debe contener un elemento task, y no puede contener otra cosa
//rigth puede contener otro elemento pipe o un elemento task
typedef struct s_pipe {
	e_nodes	type;
	char	*line;		//solo el primer nodo del arbol contendrá line!=NULL
	char	*line_extra;//Solo en caso de "ls|(vacio)" line_extra !=NULL
	t_task	*left;
	t_tree	*rigth;
}	t_pipe;

/*______________________________Execution_Structs____________________________*/

/* ... */

/*****************************************************************************/
/* 								PROTOTYPES									 */
/*****************************************************************************/

/*_____________________Parser_Prototypes______________________________________*/
//Location: main.c
e_errors	continue_cmd_tree(t_tree **tree, char **envp);
e_errors	get_cmd_tree(t_tree **tree, char **envp);
//Location: parser/constructors.c
t_task *createtask(char *segment, char *end);
t_tree *createpipe(char *line,char *pnt);
//Location parser/expansor.c
int	expandstr(char **origin, t_garbage *garbage, char *envp[]); //cuando test-expandstr no sea necesario, hacer esta funcion estatica
int	expand_vars_tree(t_tree *node, char *envp[]);
//Location: parser/expansor_utils.c
int	is_expansible(char *str);
int	count_expansions(t_task *node);
int	calculate_expansion_length(char *str, char *envp[]);
int handle_dollar(char **new_str, char **str, char **marker, char *envp[]);
//Location: parser/free_tree.c
void free_tree(t_tree *node);
//Location: parser/get_redir.c
void	get_redir(char **segment, char *end, t_redir *redir);
//Location: parser/processline.c
void	getpntword(char **segment, char *end, char **dst);
int count_cmdflags(char *segment, char *end);
int parse_task(char *segment, char *end, t_task *task);
int parsepipe(char *line, t_tree **ret);
t_tree *processline(char *line);
//Location: parser/str_utils.c
int isdelimiter(char c);
void	nullify_delimiters(char *str);
void	skipwhitesp(char **segment, char *end);
int skip_quotes(char **strpnt, char *end);
int strnchr_outquot(char **str, char *end, char c);
//Location: mooks.c
char *ft_getenv(const char *name, char *envp[]);
void print_tree(t_tree *node, int depth); //BORRAR funcion solo para pruebas
void wait_all(t_tree *node);
int executor(t_tree *node, char **envp);
//LOCATION: parser/check_tree.c
int	 check_tree(t_tree *tree, char **envp);
/*______________________________Execution_Prototypes_________________________*/

// ...

#endif

