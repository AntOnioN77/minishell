#include "../../minishell.h"
#include "../../libft/headers/libft.h"

//busca en los primeros caracteres de str una coincidencia con el nombre de una variable de entorno, 
// si el primer caracter es un separador o un espacio retorna '$', si no encuentra coincidencia retorna una cadena vacia

char *getkey(char *var)
{
	size_t len;
	char *key;

	len = ft_strchr(var, '=') - var;
	key = ft_substr(var, 0, len);
	return(key);
}

char *foundvar(char *str, char *envp[])
{
	char *validvar;
	char *namevar;
	int i;
	size_t len;

	i = 0;
	validvar = ft_strdup("");
	if (strchr(WHITESPACES,*str) || strchr(DELIMITERS,*str))
		return(ft_strdup("$"));
	while(envp[i])
	{
		namevar = getkey(envp[i]);
		if (!namevar)
			return(NULL);
		len = ft_strlen(namevar);
		if(!ft_strncmp(namevar, str, len) && len > ft_strlen(validvar))
		{
			free(validvar);
			validvar = namevar;
		}
		else
			free(namevar);
		i++;
	}
	return(validvar);
	
}

//consume marker hasta el siguiente '$', y retorna una copia de la cadena consumida.
// Si marker apuntaba a '$' consumira los caracteres a continuacion hasta encontrar
// un caracter separador (<,>,|, ,) y retornará una copia de la cadena, si la variable existe.
// si la variable no existe retorna una cadena vacia.
// si $ es seguido de un separador (por ejemplo "$|") consume $ y retorna la cadena "$\0"
char *get_piece(char **marker, char *envp[])
{
	char *piece;
	char *varname;
	size_t len;

	if(**marker == '$')
	{
		(*marker)++;
		varname = foundvar(*marker, envp);
		if (!varname)
		 return(NULL);
		if (*varname == '\0' || *varname == '$' )
			return(varname); //simplemente por que necesitamos una cadena vacia para expresar que no hay nada que concatenar cuando no se encontro un nombre valido
		return(ft_getenv(varname, envp));
	}
	else
	{
		if(ft_strchr(*marker, '$'))
			len = ft_strchr(*marker, '$') - *marker ;
		else
			len =ft_strlen(*marker);
		piece = ft_substr(*marker, 0, len);
	}
	*marker = (*marker) +len;
	return(piece);

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
	while(ft_strchr(marker, '$'))
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
	free(*origin);
	*origin = newstr;
	return (ALL_OK);
}

e_errors expansor(char **line, char **envp)
{
	char	*oldline;

	oldline = *line;
	if (is_expansible(*line))
	{
		oldline = *line;
		if (expandstr(line, envp))
		{
			free(oldline);
			return(ERROR_MALLOC);
		}
		free(oldline);
	}
	return(ALL_OK);
}
