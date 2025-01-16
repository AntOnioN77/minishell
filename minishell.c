//compilacion  cc -g3 -Wall -Wextra -Werror minishell.c expansor.c -L. -lft -lreadline

#include "minishell.h"


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
		if (skip_quotes(segment, end))
			(*segment)++;
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

//SEPARAR a getredir.c
void handle_heredoc(char **segment, char *end, t_redir *redir)
{
	(*segment) += 2;
 	if (redir)
	{
		redir->insymbol = heredoc;
		getpntword(segment, end, &(redir->infoo));
	}
	else
		getpntword(segment, end, NULL);
}

void handle_append(char **segment, char *end, t_redir *redir)
{
	(*segment) += 2;
 	if (redir)
	{
		redir->outsymbol = append;
		getpntword(segment, end, &(redir->outfile));
	}
	else
		getpntword(segment, end, NULL);
}

void handle_input(char **segment, char *end, t_redir *redir)
{
	(*segment)++;
 	if (redir)
	{
		redir->insymbol = infile;
		getpntword(segment, end, &(redir->infoo));
	}
	else
		getpntword(segment, end, NULL);
}

void handle_output(char **segment, char *end, t_redir *redir)
{
{
	(*segment)++;
 	if (redir)
	{
		redir->outsymbol = outfile;
		getpntword(segment, end, &(redir->outfile));
	}
	else
		getpntword(segment, end, NULL);
}
}

void	get_redir(char **segment, char *end, t_redir *redir)
{
	while (*segment < end)
	{
		skipwhitesp(segment, end);
		if (*segment == ft_strnstr(*segment, "<<", end - *segment))
			handle_heredoc(segment, end, redir);
		else if (*segment == ft_strnstr(*segment, ">>", end - *segment))
			handle_append(segment, end, redir);
		else if (**segment == '<')
			handle_input(segment, end, redir);
		else if (**segment == '>')
			handle_output(segment, end, redir);
		else
			return;
	}
	return ;
}
// hasta aqui separar

//NO FUNCIONA
int count_cmdflags(char *segment, char *end)
{
	int i;
	char *tmp;

	i = 0;
	while(segment < end)
	{
		get_redir(&segment, end, NULL);// aqui lo usamos solo para consumir las redirecciones, no almacenamos nada
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
		get_redir(&segment, end, &(task->redir));//si lo primero que encuentra en segment es uno o varios redir los consume, avanzando segment.
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

/*
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
			perror("processline:");
			rl_clear_history();
			free(line);
			return (1);
		}
		if(expand_tree(tree, envp))
			perror("expandtree:");//esta gestion de error es muy mejorable
//		check_tree(*tree); // tal vez implementemos esta funcion para buscar errores
//		print_tree(tree, 30);
		error = execute(tree, envp);
		free(line);
		free_tree(tree);
	}
	return (error);
}
*/
/*---------------------------EJECUTANDO_EL_ARBOL-------------------------------------------------------------------------
ªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªª*/

//int execline(t_tree)//debe ejecutar y liberar cada nodo del arbol, en los nodos typo task, y finalmente liberar el
//nodo inicial recibido.

//int check_tree() debe comprobar que ningun nodo del arbol es null. Los elementos contenidos en un nodo task, si pueden ser null.
//Si alguno de los nodos es NULL libera todo el arbol, la linea y finaliza el programa mostrando un error.