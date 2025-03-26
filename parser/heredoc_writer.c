/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_writer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-r <jperez-r@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:19:31 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/26 11:20:30 by jperez-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../executor.h"
#include <errno.h>

static t_errors	write_heredoc_line(int fd, char *separator, size_t seplen)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	signal(SIGINT, handle_sigint_heredoc);
	line = readline("> ");
	if (!line)
	{
		ft_putstr_fd("minishell: warning: here-document delimited by EOF\n", 1);
		exit (ALL_OK);
	}
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

static t_errors	write_heredoc_fork(int fd, char *separator, size_t seplen)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		return (errno);
	if (pid == 0)
		status = write_heredoc_line(fd, separator, seplen);
	else
	{
		if (waitpid(pid, &status, 0) == -1)
			return (errno);
		signal(SIGINT, handle_sigint);
		status = ((status) & 0xff00) >> 8;
	}
	return (status);
}

t_errors	heredoc_writer(char *separator, t_redir *redir)
{
	int			fd;
	size_t		seplen;
	t_errors	status;

	fd = open(redir->tmp_file, O_WRONLY | O_TRUNC);
	if (fd < 0)
		return (errno);
	seplen = ft_strlen(separator);
	status = CONTINUE;
	while (status == CONTINUE)
		status = write_heredoc_fork(fd, separator, seplen);
	if (close(fd) < 0)
		return (errno);
	if (status == E_SIGINT && unlink(redir->tmp_file) < 0)
		return (errno);
	return (status);
}
