
#include "minishell.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "../executor.h"



/*	Abre el archivo <file>, con las flags <openflag>
	Lo redirige a <fd>
	cierra el fd original de <file> */
static e_errors file_redirector(int newfd, char *file, int openflag, char **word_fail)
{
	int fd;

	if ((openflag & 0b11) == O_RDONLY)  // "openflag & 0b11" Es una mascara para obtener los 2 bits que corresponden al modo de apertura, sin flags O_RDONLY, O_WRONLY y O_RDWR
	{									// si el flag de apertura era O_RDONLY, el archivo no será creado, y por tanto debe existir y ser eccesible
		if(access(file, F_OK) == -1)
		{
			*word_fail = file;
			return (NO_EXIST);
		}
		if (access(file, W_OK == -1))
		{
			*word_fail = file;
			return (NO_PERMISSION);
		}
	}
	else //si el flag de apertura es O_RDONLY y ademas el archivo ya existe, hay que verificar que se tienen permisos de lectura. Si el archivo no existe sera creado por open mas adelante .O_RDWR queda sin gestionar pero no se usa.
	{
		if(access(file, F_OK) == 0 && access(file, R_OK == -1))
		{
			*word_fail = file;
			return (NO_PERMISSION);
		}
	}

/*
	if (openflag == O_RDONLY &&access(file, F_OK) == -1)
	{
		*word_fail = file;
		return (NO_EXIST);
		if (access(file, W_OK == -1))
		{
			*word_fail = file;
			return (NO_PERMISSION);
		}
	}
*/
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

e_errors apply_redirs(t_redir *redir, char **word_fail)
{
	e_errors error;

	error = 0;
	if (redir->insymbol == infile)
		error = file_redirector(0, redir->infoo, O_RDONLY, word_fail);
	else if (redir->insymbol == heredoc)
	{
		//signal(SIGINT, handle_sigint_heredoc);
		error = file_redirector(STDIN_FILENO, redir->tmp_file, O_RDONLY, word_fail);
	}
//	 	error = heredoc_handler(redir->infoo, redir);
	if (error != 0)
		return (error);
	
	if (redir->outsymbol == outfile)
		error = file_redirector(1 , redir->outfile, O_WRONLY | O_CREAT | O_TRUNC, word_fail);
	else if (redir->outsymbol == append)
		error = file_redirector(1 , redir->outfile, O_WRONLY | O_APPEND | O_CREAT, word_fail);
	return (error);
}
