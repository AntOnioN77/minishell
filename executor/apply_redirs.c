/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 21:17:14 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/25 02:46:55 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "executor.h"

enum e_err	file_redirector(int newfd, char *file, int opflag, char **wordfail)
{
	struct stat	tipe;
	int			fd;

	*wordfail = file;
	if ((opflag & 0b11) == O_RDONLY)
	{
		if (access(file, F_OK) == -1)
			return (NO_EXIST);
		if (access(file, W_OK == -1))
			return (NO_PERMISSION);
	}
	else if (stat(file, &tipe) == 0)
	{
		if ((tipe.st_mode & 0170000) == (0040000))
			return (IS_A_DIR);
		if (access(file, F_OK) == 0 && access(file, R_OK == -1))
			return (NO_PERMISSION);
	}
	if ((opflag & O_CREAT) == O_CREAT)
		fd = open(file, opflag, 0664);
	else
		fd = open(file, opflag);
	if (fd == -1 || dup2(fd, newfd) == -1 || close(fd))
		return (errno);
	return (ALL_OK);
}

enum e_err	apply_redirs(t_redir *redir, char **word_fail)
{
	enum e_err	error;

	error = 0;
	if (redir->insymbol == infile)
		error = file_redirector(0, redir->infoo, O_RDONLY, word_fail);
	else if (redir->insymbol == heredoc)
		error = file_redirector(STDIN_FILENO, redir->tmp_file,
				O_RDONLY, word_fail);
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
