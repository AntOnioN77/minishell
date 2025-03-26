/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-r <jperez-r@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 00:22:02 by antofern          #+#    #+#             */
/*   Updated: 2025/03/26 11:21:17 by jperez-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../executor.h"
#include <errno.h>

char	*compose_filename(int i, t_errors *error)
{
	char	*num;
	char	*str;

	num = NULL;
	num = ft_itoa(i);
	if (num == NULL)
	{
		*error = errno;
		return (NULL);
	}
	str = ft_strjoin("/tmp/.minishell.", num);
	ft_free_null((void **)&num);
	if (str == NULL)
	{
		*error = errno;
		return (NULL);
	}
	return (str);
}

char	*get_tmp_name(t_errors *error)
{
	char	*str;
	int		i;

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
	ft_putstr_fd("mini$hell: Could not name temporary file required by \
		heredoc", 2);
	*error = TMP_FILE_ERROR;
	return (NULL);
}

t_errors	create_heredoc(t_redir *redir)
{
	t_errors	error;
	char		*tmp_file;
	int			fd;

	if (redir->insymbol != heredoc)
		return (0);
	if (redir->infoo == NULL || (redir->infoo[0]) == '\0')
		return (SYNTAX_ERROR);
	error = 0;
	tmp_file = get_tmp_name(&error);
	if (error || !tmp_file)
		return (error);
	fd = open(tmp_file, O_CREAT | O_WRONLY | O_APPEND, 00600);
	if (fd == -1)
		return (errno);
	else
	{
		redir->tmp_file = tmp_file;
		close(fd);
	}
	error = heredoc_writer(redir->infoo, redir);
	return (error);
}
