//compilacion  cc -g3 minishell.c expansor.c -L. -lft -lreadline

#include "minishell.h"
//TEMPORAL TESTEO BORRAR Y REACER IMPORTANTEE!!!---------------------------------------------------------------------------------------------------------------

// Mock de ft_getenv para testing
char *ft_getenv(const char *name, char *envp[])
{
    if (strcmp(name, "VAR") == 0)
		return "value";
    if (strcmp(name, "EMPTY") == 0)
		return "";
    if (strcmp(name, "LONG") == 0)
		return "this_is_a_very_long_value";
    if (strcmp(name, "SPACE") == 0)
		return "value with spaces";
    if (strcmp(name, "QUOTES") == 0)
		return "value'with'quotes";
    return NULL;
}

//solo para test REHACER
void cleanup_garbage(t_garbage *garbage)
{
	int i;
	
	i = 0;
    while (i < garbage->current)
    {
		printf("free garbage->pointers[%d]\n", i);
        free(garbage->pointers[i]);
		i++;
    }
    free(garbage->pointers);
	
}

void free_tree(t_tree *node)
{
	t_pipe *pipe_node;
	t_task *task_node;

    if (!node)
        return;

	if (node->type == PIPE)
	{
		pipe_node = (t_pipe *)node;
		if (pipe_node->left)
			free_tree((t_tree *)pipe_node->left);
		if (pipe_node->rigth)
			free_tree(pipe_node->rigth);
		free(pipe_node);
	}
    else if (node->type == TASK)
	{
		task_node = (t_task *)node;
		cleanup_garbage(&(task_node->garb));
		// Los elementos dentro de task_node->redir y los strings 
		// en task_node->cmd y task_node->argv solo necesitan ser liberados si fueron expandidos
		// eso va a ser bastante problematico.
		// Solo liberamos el array argv
		if (task_node->argv)
			free(task_node->argv);
		free(task_node);
	}
	return ;
}

//SOLO PARA PRUEBAS imprime el arbol
void print_tree(t_tree *node, int depth)
{
    if (!node)
        return;
    for (int i = 0; i < depth; i++)
        printf("  ");
    if(node->type == PIPE)
	{
    	printf("PIPE\n");
        print_tree(((t_tree *)((t_pipe *)node)->left), depth + 1);
        print_tree(((t_pipe *)node)->rigth, depth + 1);
	}
    else if(node->type == TASK)
    {
            printf("TASK->cmd: %s\n", ((t_task *)node)->cmd);
			for (int j = 0; ((t_task *)node)->argv[j]; j++)
				printf("\t->argv[%d]: %s\n", j, ((t_task *)node)->argv[j]);
			printf("\t->redir->insymbol: %d\n", ((t_task *)node)->redir.insymbol);
			printf("\t->redir->infoo: %s\n", ((t_task *)node)->redir.infoo);
			printf("\t->redir->outsymbol: %d\n", ((t_task *)node)->redir.outsymbol);
			printf("\t->redir->outfile: %s\n", ((t_task *)node)->redir.outfile);
    }
	else
		return; //llegar aqui indicaria un error
}

//______________ FIN BORRAR__________________________________________________________________________________

/*
t_tree	*new_syntax_error(e_errors error)
{
	t_tree *ret;
	if (ret)
        ret->type = SYNTAX;
		((t_syntax *)ret)->error = error;

	ret = malloc(sizeof(t_syntax));

}
*/

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
/*
int isspecial(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1) ;
	return (0);
}
*/

int isdelimiter(char c)
{
	if (ft_strchr(DELIMITERS, c))
		return (1) ;
	return (0);
}

void	nullify_delimiters(char *str)
{
    if (!str)
        return;
    while (*str)
    {
		skip_quotes(&str, str + ft_strlen(str));
            if (isdelimiter(*str))
                *str = '\0';
        str++;
    }
}

void	skipwhitesp(char **segment, char *end)
{
	int	i;

	while (*segment != end && ft_strchr(WHITESPACES , **segment))
	(*segment)++;
	return ;
}

void	skip_redir(char **segment, char *end)
{
	//comprobar con asserts, tal vez controlar segment == NULL
	skipwhitesp(segment, end);
	while(*segment < end)
	{
		if (*segment == ft_strnstr(*segment, "<<", end - *segment))
		{
			*segment += 2;
			getpntword(segment, end, NULL);
		}
		else if (*segment == ft_strnstr(*segment, ">>", end - *segment))
		{
			*segment += 2;
			getpntword(segment, end, NULL);
		}
		else if (**segment == '<')
		{
			(*segment)++;
			getpntword(segment, end, NULL);
		}
		else if (**segment == '>')
		{
			(*segment)++;
			getpntword(segment, end, NULL);
		}
		else
			return ;
	}
}

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

int skip_quotes(char **strpnt, char *end)
{
	char *tmp;

		tmp = *strpnt;
		if(**strpnt == '"' && ft_strnchr((*strpnt) +1, '"', (end - *strpnt))) //sin  +1 strchr encontraría el propio caracter de partida
			*strpnt = ft_strnchr(*strpnt +1, '"', (end - *strpnt));
		if(**strpnt == 39 && ft_strnchr(*strpnt +1, 39, (end - *strpnt))) //39 es ' en ascii
			*strpnt = ft_strnchr(*strpnt +1, 39, (end - *strpnt));
		if (*strpnt != tmp)
		{
			(*strpnt)++;
			return (1);
		}
		return (0);
}

/*	NO TESTEADAA
	-Retorna 1 si encuentra c en str, salvo que esté encapsulado en comillas simples o dobles, o doblemente encapsulado.
	-avanza str hasta la primera coincidencia, si no encuentra chr str permanece igual
	-Probablemente no es util para buscar '\0'*/
int strnchr_outquot(char **str, char *end, char c)
{
	char *strpnt;

	strpnt = *str;
	while(strpnt <= end && *strpnt != '\0')//probablemente !='\0' innecesario
	{
		if(*strpnt == c)
		{
			*str = strpnt;
			return (1);
		}
		if(!skip_quotes(&strpnt, end))
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

//Si retorna null es un fallo de ejecución, habria que liberar todo el arbol y lanzar error
t_task *createtask(char *segment, char *end)
{
	t_task *node;

	node = malloc(sizeof(t_task));
	if (node == NULL)
		return (NULL);
	ft_bzero(node, sizeof(t_task));//free_tree() recorre todo el arbol liverando todo deteniendose en NULL, si algún elemento puntero no llegase a ser alocado permanecería muy convenientemente NULL
	node->type = TASK;
	node->argv = malloc(sizeof(char *) * (count_cmdflags(segment, end) + 1));
//	printf("numero de punteros en argv: %d\n", (count_cmdflags(segment, end) + 1));
	if (node->argv == NULL)
	{
		free(node);
		return (NULL);
	}
//Quiza es mejor una sola funcion, que vaya discriminando si la palabra apuntada por segment es un redir un comando o un argumento
//en lugar de parse_cmdflags por un lado y parsepipes por otro
	parse_task(segment, end, node);//no aloca memoria nueva solo completa campos de node. 
	return (node);
}

//NO TESTEADA
//TO DO debe liberar cosas en caso de fallo de algun malloc
//constructor de pipetree, setea izquierda con createtask(line, pnt), y derecha recurriendo a parsepipe(line +1)'
t_tree *createpipe(char *line,char *pnt)
{
	t_pipe *node;

	skipwhitesp(&line, pnt);
//	if (line == pnt)
	node = malloc(sizeof(t_pipe));
	if(node == NULL)
		return (NULL);
	ft_bzero(node, sizeof(t_pipe));
	node->type = PIPE;
	node->left = createtask(line, pnt);
	pnt++;
	if(0 == parsepipe(pnt, &(node->rigth)))//parsepipe retorna 0 si no encontro un |, si lo encontro retorna 1. Si ocurrió un error retorna 1 y pone ret=NULL
		node->rigth = (t_tree *)createtask(pnt, line + ft_strlen(line));// parsetask recibe line al completo
	return((t_tree *)node);
}


/*---------------------------PARSEOS-------------------------------------------------------------------------
ªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªª*/

//almacena un puntero a la primera letra de esa palabra, consume la siguiente palabra en segment. Si dst es NULL solamente consume un redir en segment
void	getpntword(char **segment, char *end, char **dst)
{
	skipwhitesp(segment, end);
	if (dst != NULL)
		*dst = *segment;
	while (*segment < end)
	{
		skip_quotes(segment, end);
		if(!isdelimiter(**segment))
			(*segment)++;
		else
		{
			skipwhitesp(segment, end);
			return ;
		}
	}
	return ;
}

//Temporal, de prueba
//si lo primero que encuentra en segment son redirs los consume, avanzando segment. antes de poder usar la palabra apuntada por dest, una funcion deberia recorrer line nullificando espacios y otros separadores como < o |
void	get_redir(char **segment, char *end, t_redir *redir)
{
	while (*segment < end)
	{
		skipwhitesp(segment, end);
		if (*segment == ft_strnstr(*segment, "<<", end - *segment))
		{
			redir->insymbol = heredoc;
			(*segment) += 2;
			getpntword(segment, end, &(redir->infoo));
		}
		else if (*segment == ft_strnstr(*segment, ">>", end - *segment))
		{
			redir->outsymbol = append;
			(*segment) += 2;
			getpntword(segment, end, &(redir->outfile));
		}
		else if (**segment == '<')
		{
			redir->insymbol = infile;
			(*segment)++;
			getpntword(segment, end, &(redir->infoo));
		}
		else if (**segment == '>')
		{
			redir->outsymbol = outfile;
			(*segment)++;
			getpntword(segment, end, &(redir->outfile));
		}
		else
			return;
	}
	return ;
}

//NO FUNCIONA
int count_cmdflags(char *segment, char *end)
{
	int i;
	char *tmp;

	i = 0;
	while(segment < end)
	{
		skip_redir(&segment, end);
		//skipwhitesp(&segment, end);
		tmp = segment;
		getpntword(&segment, end, NULL);
		if (tmp != segment)//si getpntword consumio algo, entonces habia una palabra
			i++;
	}

	return(i);
}


//NO TESTEADA
//si falla alocando memoria retorna 1
//get_pathname no se ha implementado aún
//add_arg no implementada
int parse_task(char *segment, char *end, t_task *task)
{
	int i;

	i = 0;
	while(segment < end)
	{
		get_redir(&segment, end, &(task->redir));//si lo primero que encuentra en segment es un redir lo consume, avanzando segment.
		if (!(segment < end))
			break ;
		if(!(task->cmd))
		{
			getpntword(&segment, end, &(task->cmd));//get_word toma la primera palabra que encuentra en segment como comando,la funcion que ejecute debe encargarse de gestionar F_OK X_OK y buscar un path.
			task->argv[0] = task->cmd; // execve por convencion recibe por argv primero el nombre del comando y despues los argumentos con que se ejecuta.
			i++;
		}
		else
		{
			getpntword(&segment, end, &(task->argv[i]));//¿debe comprobar !'\0'. Debe saltarse los espacios vacíos //MIRAR &(task...
			i++;
		}
//printf("task->argv[%d]:%s\n", i-1, task->argv[i-1]);
	}
	task->argv[i] = NULL;
	return (0);
}

int parsepipe(char *line, t_tree **ret)// desde aqui gestionar solo errores de ejecución, no de sintaxis.
{
	char *pnt;

	pnt = line;
	if(!strnchr_outquot(&pnt, pnt + ft_strlen(pnt), '|'))
		return (0);
	*ret = createpipe(line, pnt);
	return (1);
}

t_tree *processline(char *line)//debe retornar un arbol con un nodo para cada fraccion del comando introducido
{
	t_tree *ret;

	if (!ft_strcmp(line, "exit"))// EXIT PROVISIONAL!!!!!!!!
		return (NULL);//
	if(0 == parsepipe(line, &ret))//parsepipe retorna 0 si no encontro un |, si lo encontro 1. Si ocurrió un error retorna 1 y pone ret=NULL
		ret = (t_tree *)createtask(line, line + ft_strlen(line));// parsetask recibe line al completo
	//si parsetask o parsepipe falló, establecio ret a null.
	if (!ret)
		printf("Error en reserva de memoria, gestionar");//BORRAESTOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//DESCOMENTAR CUANDO SE HAGA	check_tree(ret);
	nullify_delimiters(line); //argumentos y redirecciones son punteros al string original line, esta funcion debe nulificar: whitespaces, |, <, >>, >, <<, y cualquier otro separador
	return (ret);
}



/*---------------------------PRINCIPAL-------------------------------------------------------------------------
ªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªª*/
/*
int command_flow(char **envp) //la gestion de errores de esta funcion es muy provisional
{
	char 	*line;
	t_tree	*tree;
	int		error;

	error = 0;
	while(error = 0)
	{
		line = readline("mini$hell>");
		if(!line)
		{
			perror("readline:");
			return (1);
		}
		tree = processline(line);
		if (tree == NULL)
		{
			free(line);
			return (1);
		}
		error = execline(tree, envp);
		if (error) //execline debe liberar los nodos desde las hojas hacia arriba. 
		{
			free(line);
			free_tree(tree);
		}
	}
	return (error);
}

*/

int main(int argc, char **argv, char **envp)
{
	char 	*line;
	t_tree	*tree;
	int		error;
	char	**new_envp;
	
	error = 0;
	while(error == 0)
	{
		line = readline("mini$hell>");
		if(!line)
		{
			perror("readline:");
			return (1);
		}
		tree = processline(line);
		if (tree == NULL)
		{
			rl_clear_history();
			free(line);
			return (1);
		}
		expand_tree(tree, envp);
//		check_tree(*tree); // tal vez implementemos esta funcion para buscar errores
		print_tree(tree, 30);// SUSTITUYE  esta funcion por la función que se encarga de la ejecucion del arbol.
		free(line);
		free_tree(tree);
	}
}

/*---------------------------EJECUTANDO_EL_ARBOL-------------------------------------------------------------------------
ªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªª*/

//int execline(t_tree)//debe ejecutar y liberar cada nodo del arbol, en los nodos typo task, y finalmente liberar el
//nodo inicial recibido.

//int check_tree() debe comprobar que ningun nodo del arbol es null. Los elementos contenidos en un nodo task, si pueden ser null.
//Si alguno de los nodos es NULL libera todo el arbol, la linea y finaliza el programa mostrando un error.