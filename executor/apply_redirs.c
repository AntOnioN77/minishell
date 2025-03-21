
#include "minishell.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../executor.h"



/*	Abre el archivo <file>, con las flags <openflag>
	Lo redirige a <fd>
	cierra el fd original de <file> */
e_errors	file_redirector(int newfd, char *file, int opflag, char **wordfail)
{
	struct stat	tipe;
	int			fd;

	*wordfail = file;//en caso de que falle ya esta seteado
	if ((opflag & 0b11) == O_RDONLY)// "openflag & 0b11" Es una mascara para obtener los 2 bits que corresponden al modo de apertura, sin flags O_RDONLY, O_WRONLY y O_RDWR
	{									// si el flag de apertura era O_RDONLY, el archivo no será creado, y por tanto debe existir y ser accesible
		if (access(file, F_OK) == -1)
			return (NO_EXIST);
		if (access(file, W_OK == -1))
			return (NO_PERMISSION);
	}
	//si el flag de apertura es O_RDONLY y ademas el archivo ya existe, hay que verificar que se tienen permisos de lectura. Si el archivo no existe sera creado por open mas adelante
	//.O_RDWR queda sin gestionar pero no se usa.
	else if (stat(file, &tipe) == 0)
	{
//fprintf(stderr,"Valor st_mode: %o\n", tipe.st_mode);
//fprintf(stderr,"Resultado de máscara: %o\n", (tipe.st_mode & 0170000));
//fprintf(stderr,"Comparación: %d\n", ((tipe.st_mode & 0170000) == 0040000));
		if ((tipe.st_mode & 0170000) == (0040000))
			return (IS_A_DIR);
		if (access(file, F_OK) == 0 && access(file, R_OK == -1))
			return (NO_PERMISSION);
	}
//creo que esto se va
	if ((opflag & O_CREAT) == O_CREAT)
		fd = open(file, opflag, 0664);
	else
		fd = open(file, opflag); /////////////// fin se va
	if (fd == -1 || dup2(fd, newfd) == -1 || close(fd))
		return (errno);
	return (ALL_OK);
}

e_errors	apply_redirs(t_redir *redir, char **word_fail)
{
	e_errors	error;

	error = 0;
	if (redir->insymbol == infile)
		error = file_redirector(0, redir->infoo, O_RDONLY, word_fail);
	else if (redir->insymbol == heredoc)
		error = file_redirector(STDIN_FILENO, redir->tmp_file,
				O_RDONLY, word_fail);
//	 	error = heredoc_handler(redir->infoo, redir);
	if (error != 0)
		return (error);
	if (redir->outsymbol == outfile)
		error = file_redirector(1, redir->outfile,
				O_WRONLY | O_CREAT | O_TRUNC, word_fail);
	else if (redir->outsymbol == append)
		error = file_redirector(1, redir->outfile,
				O_WRONLY | O_APPEND | O_CREAT, word_fail);
	return (error);
}
