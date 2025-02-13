#include "minishell.h"
#include "GNL/get_next_line.h"

int	matrixlen(char **matrix)
{
	size_t	len;

	len = 0;
	while (matrix[len])
		len++;
	return (len);
}

int		get_histsize(const char *histsize)
{
	const char *histfilesize;
	if (!histsize)
	{
		histfilesize = getenv("HISTFILESIZE");
		if (histfilesize)
        	return (ft_atol(histfilesize));
		else
			return ((size_t)DEFAULT_HISTSIZE);
	}
	else
		return (ft_atol(histsize));
}
/**
 * Se va a crear el límite de comandos que puede tener el historial en el file
 */
void	save_limit_history(char *hist, int fd)
{
	size_t	limit;
	size_t	hsize;
	char	**hists;

	//Hay que jugar con el límite de líneas que puede tener bash en el historial
    limit = get_histsize(getenv("HISTSIZE"));
	hists = ft_split(hist, '\n');
	hsize = matrixlen(hists);

	//rl_clear_history();
	//
	printf("limit: %zu\n", limit);
	printf("hsize: %zu\n", hsize);
	
	if (hsize <= limit)
		hsize = 0;
	else
		hsize -= limit; 
	//if (fd)
	printf("limit: %zu\n", limit);
	//printf("hsize: %zu\n", hsize);
	while (hists[hsize] && fd)
	{
		add_history(hists[hsize]);
		//write (fd, hists[hsize], ft_strlen(hists[hsize]));
		hsize++;
	}
	//printf("hola\n");
	//close(fd);
	//return
		//exit(0);
}

void	load_history(void)
{
	
	int		fd;
	int		reader;
	char	line[1024];
	char	*hist;

	//Comprobar si existe el archivo y es accesible
	if (access (HISTORY_FILE, F_OK | R_OK | W_OK) != 0)
		return ;
	//Abrir el archivo si existe 
	fd = open(HISTORY_FILE, O_RDWR);// | O_TRUNC);

	//GNL
	if (fd)
	{
		reader = 1;
		//hist = NULL;
		//Habrá que contar con el límite del historial
		while (reader)
		{
			reader = read(fd, line, 1024);
			//if (reader > 0)
			//printf("%d\n", reader);
			if (reader > 0)
				hist = ft_strjoin(hist, line);
			
		//printf("hla");
			//free(line);
		}
		if (hist)
			save_limit_history(hist, fd);
			//printf("hla");
			//printf("%s\n", hist);
		//Cerrar el archivo
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
	int		fd;

	add_history(history);
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
