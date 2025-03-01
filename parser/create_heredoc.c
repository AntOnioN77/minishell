#include "../minishell.h"
#include "../executor.h"
#include <errno.h>
#include "../GNL/get_next_line.h"

char *compose_filename(int i, e_errors *error)
{
	char *num;
	char *str;

	num = NULL;
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
	return(str);
}
//#include <fcntl.h>
//#include <unistd.h>
//#include "get_next_line.h"

//la linea creada, almacenada en redir->tmp_file debe ser liberada (esta sin implementar)
char *get_tmp_name(e_errors *error)
{
	char *str;
	int i;

	i = 0;

	str = NULL;
	while (i < 500)
	{
		str = compose_filename(i, error);
		if (access(str, F_OK) != 0 && errno == ENOENT)
			return (str);
		i++;
		ft_free_null((void **)&str);
	}
	ft_putstr_fd("mini$hell: Could not name temporary file required by heredoc", 2);
	*error = TMP_FILE_ERROR;
	return (NULL);
}


static e_errors write_heredoc_line(int fd, char *separator, size_t seplen)
{
	char	*line;

	signal(SIGINT, handle_sigint_heredoc);
	line = readline("");
	if (!line)
		return (errno);
	if (ft_strlen(line) == seplen && !ft_strncmp(line, separator, seplen))
	{
		free(line);
		exit (ALL_OK);
	}
	ft_putstr_fd(line, fd);
	ft_putchar_fd('\n', fd);
	free(line);
	exit (CONTINUE);
}

static e_errors write_heredoc_fork(int fd, char *separator, size_t seplen)
{
	//char	*str_status;
	pid_t	pid;
	int		status;

	//status = 0;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (ERROR); //Habrá que devolver el error correspondiente al fork
	}
	if (pid == 0)
		status = write_heredoc_line(fd, separator, seplen);
	else
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("waitpid");
			return (ERROR); // igual que el fork, pero en waitpid
		}
		status = ((status) & 0xff00) >> 8;
		//return (((status) & 0xff00) >> 8);
		// str_status = ft_itoa(((status) & 0xff00) >> 8);//aplicamos mascara (WEXISTATUS)
		// change_var("?", str_status , environ);
	}
	return (status);
}

e_errors heredoc_writer(char *separator, t_redir *redir)
{
	int fd;
	size_t seplen;
	e_errors status;

	fd = open(redir->tmp_file, O_WRONLY | O_TRUNC);
	if (fd < 0)
		return (errno);
	seplen = ft_strlen(separator);
	while (1)
	{
		status = write_heredoc_fork(fd, separator, seplen);
		if (status != CONTINUE)
		{
			close(fd);
			return (status);
		}
	}
	close(fd);
	if (status == E_SIGINT)
		return (E_SIGINT);
	return (ALL_OK);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//el archivo creado debe ser eliminiado en free_tree, no implementado aun
e_errors create_heredoc(t_redir *redir)
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

	error = heredoc_writer(redir->infoo, redir);
	return(error);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
