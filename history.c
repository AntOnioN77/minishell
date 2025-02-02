#include "minishell.h"

void	charge_history(const char *histsize)
{
	unsigned int	size;
	//char	*pathname = "./history";
	int		fd;
	char	buff[100000];

    if (histsize)
    	size = atol(histsize);
    else
        size = DEFAULT_HISTSIZE;
	//comprobar si existe el archivo y es accesible
	if (access (HISTORY_FILE, F_OK | R_OK) != 0)
		return ;
	//abrir el archivo si existe 
	fd = open(HISTORY_FILE, O_RDONLY);
	//leer el archivo / GNL
	if (fd)
	{
		//while ()
		read (fd, buff, 99999);
		//cerrar el archivo
		close (fd);
	}
}
/**
 * Hay que gestionar el número de comandos que guardará history. Por defecto,
 * en nuestro caso, son 500. Se puede comprobar usando el comando echo $HISTSIZE
 * en el terminal.
 * Como es algo que se puede modificar temporalmente con el comando
 * "export HISTSIZE=2000", habría que limitar el número a lo que haya guardado
 * en esta variable
 */
int	save_history(char *history)
{
	//char	*pathname = "./history";
	int		fd;
	//int		res;

	//comprobar si existe el archivo y es accesible
	if (access (HISTORY_FILE, F_OK) != 0)
	//crear el archivo si no existe y abrirlo
		fd = open(HISTORY_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	else
	{ 
		if (access (HISTORY_FILE, W_OK) == 0)
			return (1); //error archivo sin permisos para escribir. VER en BASH
		fd = open(HISTORY_FILE, O_WRONLY);
	}
	if (fd)
	{
		//spbrescribir o hacer append en el archivo
		write (fd, history, ft_strlen(history));
		//cerrar el archivo
		close (fd);
	}
	return (0);
}
