#include "../../minishell.h"
#include "../../libft/headers/libft.h"

//consume marker hasta el siguiente '$', y retorna una copia de la cadena consumida.
// Si marker apuntaba a '$' consumira los caracteres a continuacion hasta encontrar
// un caracter separador (<,>,|, ,) y retornará una copia de la cadena, si la variable existe.
// si la variable no existe retorna una cadena vacia.
// si $ es seguido de un separador (por ejemplo "$|") consume $ y retorna la cadena "$\0"
char *get_piece(char **marker, char *envp[])
{
	char *piece;

	piece = ft_substr(*marker, 0, );

}

e_errors	expandstr(char **origin, char *envp[])
{
	char *newstr;
	char *auxstr;
	char *piece;
	char *marker;

	marker = *origin;
	newstr = get_piece(&marker, envp);
	if(!newstr)
		return (ERROR_MALLOC);
	while(strchr(marker, '$'))
	{
		piece = get_piece(&marker, envp);
		if(!piece)
			return (ERROR_MALLOC);
		//anadir fragmento a la cadena
		auxstr = newstr;
		newstr = ft_strjoin(newstr, piece);
		free(piece);
		free(auxstr);
		if(!newstr)
			return (ERROR_MALLOC);
	}
	free(origin);
	*origin = newstr;
	return (ALL_OK);
}