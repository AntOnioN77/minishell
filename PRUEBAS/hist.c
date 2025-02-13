#include "minishell.h"
#include "GNL/get_next_line.h"

int matrixlen(char **matrix)
{
    size_t  len;

    len = 0;
    while (matrix[len])
        len++;
    return (len);
}

int     get_histsize(const char *histsize)
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
/**
 * Se va a crear el límite de comandos que puede tener el historial en el file
 */
void    save_limit_history(char *hist, int fd)
{
    size_t  limit;
    size_t  hsize;
    char    **hists;

    //Hay que jugar con el límite de líneas que puede tener bash en el historial
    limit = get_histsize(getenv("HISTSIZE"));
    hists = ft_split(hist, '\n');
    hsize = matrixlen(hists);
    //
    if (hsize >= limit)
        hsize = 0;
    else
        hsize -= limit;
    while (hists[limit])
    {
        add_history(hists[limit]);
        write (fd, hists[limit], ft_strlen(hists[limit]));
        limit++;
    }
    close(fd);
    //return
        exit(0);
}

void    load_history(void)
{

    int     fd;
    int     reader;
    char    line[1024];
    char    *hist;

    //Comprobar si existe el archivo y es accesible
    if (access (HISTORY_FILE, F_OK | R_OK | W_OK) != 0)
        return ;
    //Abrir el archivo si existe
    fd = open(HISTORY_FILE, O_RDWR | O_TRUNC);
    //GNL
    if (fd)
    {
        reader = 1;
        //Habrá que contar con el límite del historial
        while (reader)
        {
            reader = read(fd, line, 1024);
            hist = ft_strjoin(hist, line);
            free(line);
        }
        save_limit_history(hist, fd);
        //Cerrar el archivo
        close (fd);
    }
}

/*void  load_history(void)
{
    unsigned int    size;
    //char  *pathname = "./history";
    int     fd;
    char    buff[1024];
    int     reader;

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
int save_history(char *history)
{
    //char  *pathname = "./history";
    int     fd;
    //int       res;

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