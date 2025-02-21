
#include "minishell.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "../executor.h"



/*	Abre el archivo <file>, con las flags <openflag>
	Lo redirige a <fd>
	cierra el fd original de <file> */
static e_errors file_redirector(int newfd, char *file, int openflag)
{
	int fd;

//	if(!file || *file == '\0')
//		return(SYNTAX_ERROR);
	if ((openflag & O_CREAT) == O_CREAT)
		fd = open(file, openflag, 0664);
	else
		fd = open(file, openflag);
	if (fd == -1 || dup2(fd, newfd) == -1 || close(fd))
		return(errno);
	return(ALL_OK);
}

e_errors apply_redirs(t_redir *redir)
{
	e_errors error;

	error = 0;
	if (redir->insymbol == infile)
		error = file_redirector(0, redir->infoo, O_RDONLY);
	else if (redir->insymbol == heredoc)
		error = file_redirector(STDIN_FILENO, redir->tmp_file, O_RDONLY);
//	 	error = heredoc_handler(redir->infoo, redir);
	if (error != 0)
		return (error);
	
	if (redir->outsymbol == outfile)
		error = file_redirector(1 , redir->outfile, O_WRONLY | O_CREAT | O_TRUNC);
	else if (redir->outsymbol == append)
		error = file_redirector(1 , redir->outfile, O_WRONLY | O_APPEND | O_CREAT);
	return (error);
}