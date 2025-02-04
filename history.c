#include "minishell.h"
#include "GNL/get_next_line.h"

int		get_histsize(const char *histsize)
{
	const char *histfilesize;
	if (!histsize)
	{
		histfilesize = getenv("HISTFILESIZE");
		if (histfilesize)
        	return (ft_atol(histfilesize));
		else
			return (DEFAULT_HISTSIZE);
	}
	else
		return (ft_atol(histsize));
}

void	load_history(void)
{
	unsigned int	size;
	int		fd;
	char	*line;

	//Hay que jugar con el límite de líneas que puede tener bash en el historial
    size = get_histsize(getenv("HISTSIZE"));
	//comprobar si existe el archivo y es accesible
	if (access (HISTORY_FILE, F_OK | R_OK) != 0)
		return ;
	//abrir el archivo si existe 
	fd = open(HISTORY_FILE, O_RDONLY);
	//GNL
	if (fd)
	{
		line = get_next_line(fd);
		//Habrá que contar con el límite del historial
		while (line)
		{
			add_history(line);
			free(line);
			line = get_next_line(fd);
		}
		//cerrar el archivo
		close (fd);
	}
}

/*void	load_history(void)
{
	unsigned int	size;
	//char	*pathname = "./history";
	int		fd;
	char	buff[1024];
	int		reader;

	//Hay que jugar con el límite de líneas que puede tener bash en el historial
    size = get_histsize(getenv("HISTSIZE"));

	//comprobar si existe el archivo y es accesible
	if (access (HISTORY_FILE, F_OK | R_OK) != 0)
		return ;
	//abrir el archivo si existe 
	fd = open(HISTORY_FILE, O_RDONLY);
	//leer el archivo / GNL
	if (fd)
	{
		reader = 1;
		while (reader > 0)
		{
			reader = read (fd, buff, sizeof(buff) - 1);

		}
		//cerrar el archivo
		close (fd);
	}
}*/
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
		fd = open(HISTORY_FILE, O_WRONLY | O_APPEND);
	}
	if (fd)
	{
		//sobrescribir o hacer append en el archivo
		write (fd, history, ft_strlen(history));
		//cerrar el archivo
		close (fd);
	}
	return (0);
}
