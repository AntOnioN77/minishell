
//int execline(t_tree)//debe ejecutar y liberar cada nodo del arbol, y finalmente liberar el nodo inicial recibido

//execline

#include "libft/headers/libft.h"



typedef struct s_token
{
	char *start;
	char *end;
} t_token;


/*types 
	frase	"
	frase2	'
	redir	<
	redir	>
	redir 	>> +
	redir	<< H
	pipe	|
*/





typedef struct s_tree
{
	char type;
	t_tree *origin;
	t_token token;
	t_tree *rigth;
	t_tree *left;
} t_tree;

#define SYMBOLS "\"\'<>|"
#define WHITESPACES " \r\n\v\t"


/*-Avanza str hasta agotar espacios en blanco iniciales. Despues
comprueba si el nuevo caracter apuntado por str, es alguno de los caracteres
que contiene la cadena <wanted>, en caso afirmativo retorna 1. */
int isthere(char **segment, char *endseg, char *wanted)
{
	while(*segment > endseg && ft_strchr(WHITESPACES, **segment))
		*segment++;
	if (ft_strchr(wanted, **segment))
	 	return (1);
	return (0);
}


static int chop(char *segment, char *endseg, t_tree *tree)
{
	char *endleft;

	isthere(segment, endseg, SYMBOLS);

	next_in_precedence

	setnode(segment, endleft, tree)

}



t_tree parseline(char *line)//debe retornar un arbol con un nodo para cada fraccion del comando introducciodo
{
	t_tree ret;

	if (chop(line, line[ft_strlen(line)], &ret))
		error();//TO DO:gestionar
	
	//to do

	return (ret);
}

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
		execline(parseline(line)); //execline debe liberar los nodos desde las hojas hacia arriba.
		free(line);
	}
}

int main(int argc, char **argv, char **envp)
{
	upgrade_envp(envp);//no necesita liverar envp anterior despues de clonarlo, pues "env" no esta reservado dinamicamente
	return(command_flow(envp));
}