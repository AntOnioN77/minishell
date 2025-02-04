
#include "minishell.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "get_next_line.h"
#include "../executor.h"



/*	Abre el archivo <file>, con las flags <openflag>
	Lo redirige a <fd>
	cierra el fd original de <file> */
static e_errors file_redirector(int newfd, char *file, int openflag)
{
	int fd;

	if ((openflag & O_CREAT) == O_CREAT)
		fd = open(file, openflag, 0664);
	else
		fd = open(file, openflag);
	if (fd == -1 || dup2(fd, newfd) || close(fd))
		return(errno);
	return(ALL_OK);
}

static e_errors heredoc_handler(char *separator, t_redir *redir)//char *separator)
{
	int fd;
	char *line;
	int seplen;
	e_errors error;

//NOTA : gestionar este error sintactico desde check_tree!!
/*
	if(!(redir->infoo))
	{
		ft_putstr_fd(" syntax error: (<<) requires a separator", 2);
		return(SYNTAX_ERROR);
	}
*/
	//pedir nueva linea mientras linea no sea == separator
	///////////////ABSTRAER///////////////////////////////////////
	seplen = ft_strlen(separator);
	line = get_next_line(0);
	if(!line)
		return(errno);	
	while(ft_strncmp(line, separator, seplen) && line)
	{
		ft_putstr_fd(line, fd);
		free(line);
		line = get_next_line(0);
	}
	if(line)
		free(line);
	/////////////////////////FIN ABSTRAER/////////////////////////
	// redirigir STDIN a un fd que apunte al principio del archivo temporal
	close(fd);
	error = file_redirector(STDIN_FILENO, redir->tmp_file, O_RDONLY);
	return(error);

}

e_errors apply_redirs(t_redir *redir)
{
	e_errors error;

	error = 0;
	if (redir->insymbol == infile)
		error = file_redirector(0, redir->infoo, O_RDONLY);
	else if (redir->insymbol == heredoc)
	 	error = heredoc_handler(redir->infoo, redir);
	
	if (error != 0)
		return (error);
	
	if (redir->outsymbol == outfile)
		error = file_redirector(1 , redir->outfile, O_WRONLY | O_CREAT);
	else if (redir->outsymbol == append)
		error = file_redirector(1 , redir->outfile, O_WRONLY | O_APPEND | O_CREAT);
	return (error);
}