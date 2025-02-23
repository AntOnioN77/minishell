#include "../minishell.h"
#include "../executor.h"
#include <errno.h>
#include "../GNL/get_next_line.h"

//#include <fcntl.h>
//#include <unistd.h>
//#include "get_next_line.h"

//la linea creada, almacenada en redir->tmp_file debe ser liberada (esta sin implementar)
char *get_tmp_name(e_errors *error)
{
	char *str;
	char *num;
	int i;

	i = 0;

	num = NULL;
	str = NULL;
	while (i < 500)
	{
		num = ft_itoa(i);
		if (num == NULL)
		{
			*error = errno;
			return(NULL);
		}
		str = ft_strjoin("/tmp/.minishell.", num);
		ft_free_null((void **)&num);
		if(str == NULL)
		{
			*error = errno;
			return(NULL);
		}
		if (access(str, F_OK) != 0 && errno == ENOENT)
			return (str);
		i++;
		ft_free_null((void **)&str);
	}
	ft_putstr_fd("mini$hell: Could not name temporary file required by heredoc", 2);
	*error = TMP_FILE_ERROR;
	return (NULL);
}


e_errors heredoc_writer(char *separator, t_redir *redir)//char *separator)
{
	int fd;
	char *line;
	size_t seplen;
	e_errors error;

	error = 0;
	fd = open(redir->tmp_file, O_WRONLY | O_TRUNC);

	//pedir nueva linea mientras linea no sea == separator
	///////////////ABSTRAER///////////////////////////////////////
	seplen = ft_strlen(separator);
	while(1)
	{
		if (g_ctrlc == SIGINT)
		{
		// fprintf(stdout, "%d\n", g_ctrlc);
			close(fd);
			g_ctrlc = 0;
			return (E_SIGINT);
		}
		//get_next_line(0, &line); //ni readline ni get_next_line retornan NULL cuando esperan entrada y pulsas ctrl+c
			line = readline(""); //dejo readline por que su still reachable esta justificado por subject
//fprintf(stderr, "get_next_line:%s\n", line);
//fprintf(stderr, "get_next_line:%p\n", line);
		if (!line)
		{
			close(fd);
fprintf(stderr, "66\n");				//ctrl+c ?????????
			return(errno);
		}
		if(ft_strlen(line) == seplen && !ft_strncmp(line, separator, seplen))
		{
			free(line);
			break ;
		}
		else // No se necesita si se hace un break
			ft_putstr_fd(line, fd);
		ft_putchar_fd('\n', fd);
		free(line);

	}
	/////////////////////////FIN ABSTRAER/////////////////////////
	// redirigir STDIN a un fd que apunte al principio del archivo temporal
	close(fd);
	//error = file_redirector(STDIN_FILENO, redir->tmp_file, O_RDONLY);
	return(error);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//el archivo creado debe ser eliminiado en free_tree, no implementado aun
e_errors create_herefile(t_redir *redir)
{
	e_errors error;
	char *tmp_file;
	int fd;

	if(redir->insymbol != heredoc)
		return (0);
	error = 0;
	tmp_file = get_tmp_name(&error);
	if (error || !tmp_file)
		return (error);
	//crear archivo temporal
	fd = open(tmp_file, O_CREAT | O_WRONLY | O_APPEND, 00600);
	if(fd == -1)
		return(errno);
	else
	{
		redir->tmp_file = tmp_file;
		close(fd);
	}

	signal(SIGINT, handle_sigint_heredoc);
	error = heredoc_writer(redir->infoo, redir);
	//signal(SIGINT, handle_sigint);
	return(error);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////