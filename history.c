#include "minishell.h"

void	charge_history()
{
	char	*pathname = "./history";
	int		fd;
	char	buff[100000];

	//comprobar si existe el archivo y es accesible
	if (access (pathname, F_OK | R_OK) != 0)
		return ;
	//abrir el archivo si existe 
	fd = open(pathname, O_RDONLY);
	//leer el archivo / GNL
	if (fd)
	{
		read (fd, buff, 99999);
		//cerrar el archivo
		close (fd);
	}
}

int	save_history(char *history)
{
	char	*pathname = "./history";
	int		fd;
	//int		res;

	//comprobar si existe el archivo y es accesible
	if (access (pathname, F_OK) != 0)
	//crear el archivo si no existe y abrirlo
		fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	else
	{ 
		if (access (pathname, W_OK) == 0)
			return (1); //error archivo sin permisos para escribir. VER en BASH
		fd = open(pathname, O_WRONLY);
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
