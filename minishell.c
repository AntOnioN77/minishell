#include "libft/headers/libft.h"

typedef enum e_nodes
{
	none,
	line,//probablemente innecesario
	pipe,
	redir,//probablemente innecesario
	task
}	e_nodes;

typedef enum e_symbols
{

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
	char	*pathname;
	char	*argv[];
}	t_task;

typedef struct s_pipe
{
	e_nodes	type;
	t_task	*left;
	t_tree	*rigth;
}	t_pipe;

t_tree *processline(char *line);
#define WHITESPACES " \r\n\v\t"

/*---------------------------STRING_UTILITIES-------------------------------------------------------------------------
ªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªª*/
/*-Avanza str hasta agotar espacios en blanco iniciales. Despues
comprueba si el nuevo caracter apuntado por str, es alguno de los caracteres
que contiene la cadena <wanted>, en caso afirmativo retorna 1. */
/*int isitnext(char **segment, char *endseg, char *wanted)
{
	while(*segment > endseg && ft_strchr(WHITESPACES, **segment))
		*segment++;
	if (ft_strchr(wanted, **segment))
	 	return (1);
	return (0);
}*/

//NO TESTEADA
char	*ft_strnchr(const char *s, int c, int n)
{
	int				i;
	unsigned char	uc;

	uc = (unsigned char)c;
	i = 0;
	while (i < n && s[i] && (unsigned char)s[i] != uc)
	{
		i++;
	}
	if (s[i] == uc)
		return ((char *)&s[i]);
	else
		return (NULL);
}

/*	NO TESTEADAA
	-Retorna 1 si encuentra c en str, salvo que esté encapsulado en comillas simples o dobles, o doblemente encapsulado.
	-avanza str hasta la primera coincidencia, si no encuentra chr str permanece igual
	-Probablemente no es util para buscar '\0'*/
int strnchr_outquot(char **str, char *end, char c)
{
	char *strpnt;

	strpnt = *str;
	while(*strpnt <= end && *strpnt != '\0')//probablemente !='\0' innecesario
	{
		if(*strpnt == c)
		{
			*str = strpnt;
			return (1);
		}
		if(*strpnt == '"' && ft_strnchr(strpnt +1, '"', (end - strpnt))) //sin  +1 strchr encontraría el propio caracter de partida
			strpnt = ft_strnchr(strpnt +1, '"', (end - strpnt));
		if(*strpnt == 39 && ft_strnchr(strpnt +1, 39, (end - strpnt))) //39 es ' en ascii
			strpnt = ft_strnchr(strpnt +1, 39, (end - strpnt));
		strpnt++;
	}
	return (0);
}

//skip_redir(char **segment, char *end) //si lo primero que contiene segment es uno o varioss redirs, situa segment despues y retorna 1. Si no, retorna 0 y dejasegment donde estaba.

//char *get_patname() MIRAR EN PIPEX reciclable?
//char *get_cmdargs()

/*---------------------------CONSTRUCTORES-------------------------------------------------------------------------
ªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªª*/

//t_redir *createredir

t_tree *createtask(char *segment, char *end)
{
	t_task *task;

	task = malloc(sizeof(t_task));
	parseredirs(segment, end, task);
	parse_cmdflags(segment, end, task);//

}

//constructor de pipetree, setea izquierda con createtask(line, pnt), y derecha recurriendo a parsepipe(line +1)'
t_tree *createpipe(line, pnt, head)
{
	t_pipe *node;

	node = malloc(sizeof(t_pipe));
	if(node = NULL)
		return (NULL);
	ft_bzero(node, sizeof(t_pipe));
	node->type = pipe;
	node->left = createtask(line, pnt);
	if(0 == parsepipe(pnt, node->rigth))//parsepipe retorna 0 si no encontro un |, si lo encontro retorna 1. Si ocurrió un error retorna 1 y pone ret=NULL
		node->rigth = createtask(line, line + ft_strlen(line));// parsetask recibe line al completo
	return((t_tree *)node);
}


/*---------------------------PARCES: me queé to sanahorio-------------------------------------------------------------------------
ªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªª*/

parse_cmdflags(char *segment, char *end, t_task *task)
{
	skipredirs(&segment, end);
	while(segment < end)
	{
		if(!(task->pathname))
			task->pathname = *get_patname();
		else
			add_arg(task->argv);
		skipredirs(&segment, end);
	}
}

int parsepipe(char *line, t_tree **ret)// desde aqui gestionar solo errores de ejecución, no de sintaxis.
{
	char *pnt;

	pnt = line;
	if(!strnchr_outquot(&pnt, '|', ft_strlen(pnt)))
		return (0);
	ret = createpipe(line, pnt, ret);
	return (1);
}

t_tree *processline(char *line)//debe retornar un arbol con un nodo para cada fraccion del comando introducido
{
	t_tree *ret;

	if(0 == parsepipe(line, &ret))//parsepipe retorna 0 si no encontro un |, si lo encontro 1. Si ocurrió un error retorna 1 y pone ret=NULL
		ret = createtask(line, line + ft_strlen(line));// parsetask recibe line al completo
	//si parsetask o parsepipe falló, establecio ret a null.
	if (!ret)
		//TO DO liberar algo?
	return (ret);
}



/*---------------------------PRINCIPAL-------------------------------------------------------------------------
ªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªª*/

int command_flow(char **envp)
{
	char *line;

	while(1)
	{
		line = readline("mini$hell>");
		if(!line)
		{
			perror("readline:");
			break;
		}
		if (execline(processline(line))) //execline debe liberar los nodos desde las hojas hacia arriba. 

		free(line);
	}
}

int main(int argc, char **argv, char **envp)
{
	int	outstate;

	upgrade_envp(envp);//no necesita liverar envp anterior despues de clonarlo, pues "env" no esta reservado dinamicamente
	outstate = command_flow(envp);
	free(envp);
	return(outstate);
}



/*---------------------------EJECUTANDO_EL_ARBOL-------------------------------------------------------------------------
ªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªª*/

//int execline(t_tree)//debe ejecutar y liberar cada nodo del arbol, en los nodos typo task, y finalmente liberar el nodo inicial recibido