
//int execline(t_tree)//debe ejecutar y liberar cada nodo del arbol, y finalmente liberar el nodo inicial recibido

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
/*int isitnext(char **segment, char *endseg, char *wanted)
{
	while(*segment > endseg && ft_strchr(WHITESPACES, **segment))
		*segment++;
	if (ft_strchr(wanted, **segment))
	 	return (1);
	return (0);
}
*/

/*	NO TESTEADAAA
	-Retorna 1 si encuentra c en str, salvo que esté encapsulado en comillas simples o dobles, o doblemente encapsulado.
	-avanza str hasta la primera coincidencia, si no encuentra chr str permanece igual*/
int strchr_outquot(char **str, char c)
{
	bool dquote;
	bool squote;
	int i;
	char *strpnt;

	strpnt = *str;
	dquote = FALSE;
	squote = FALSE;
	i = 0;
	while(*strpnt != '\0')
	{
		if(*strpnt == c && !squote && !dquote)
		{
			*str = strpnt;
			return (1);
		}
		if(!squote && *strpnt == '"' ) // cuando squote==TRUE strpnt estara apuntando a un caracter anidado y por tanto irrelevante
			dquote = !dquote;
		if(!dquote  && *strpnt == 39) //BORRAESTO ascii 39 = ' 
			squote = !squote;
		strpnt++;
	}
}


int parsepipe(char *line, t_tree **ret)
{
	

}

t_tree parseline(char *line)//debe retornar un arbol con un nodo para cada fraccion del comando introducido
{
	t_tree *ret;

	if(0 == parsepipe(line, &ret) && ret == NULL)//parsepipe retorna 0 si no ocurrio un error, si ocurrio un error retorna 1 y pone ret=NULL
		ret = parsetask(line);
	//si parsetask o parsepipe falló, establecio ret a null.
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
		if (execline(parseline(line))) //execline debe liberar los nodos desde las hojas hacia arriba. 

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