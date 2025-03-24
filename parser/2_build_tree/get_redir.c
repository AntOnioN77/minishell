/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:11:59 by antofern          #+#    #+#             */
/*   Updated: 2025/03/25 00:06:09 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <errno.h>

static void	handle_append(char **segment, char *end, t_redir *redir)
{
	(*segment) += 2;
	if (redir)
	{
		create_file(*segment, end, O_APPEND, redir);
		redir->outsymbol = append;
		getpntword(segment, end, &(redir->outfile));
	}
	else
		getpntword(segment, end, NULL);
	return ;
}

static void	handle_heredoc(char **segment, char *end, t_redir *redir)
{
	(*segment) += 2;
	if (redir)
	{
		redir->insymbol = heredoc;
		getpntword(segment, end, &(redir->infoo));
		if (!redir->infoo || *(redir->infoo) == '\0')
			redir->error = SYNTAX_ERROR;
	}
	else
		getpntword(segment, end, NULL);
	return ;
}

static void	handle_input(char **segment, char *end, t_redir *redir)
{
	(*segment)++;
	if (redir)
	{
		check_file(*segment, end, redir);
		redir->insymbol = infile;
		getpntword(segment, end, &(redir->infoo));
	}
	else
		getpntword(segment, end, NULL);
}

static void	handle_output(char **segment, char *end, t_redir *redir)
{
	{
		(*segment)++;
		if (redir)
		{
			create_file(*segment, end, O_TRUNC, redir);
			redir->outsymbol = outfile;
			getpntword(segment, end, &(redir->outfile));
		}
		else
			getpntword(segment, end, NULL);
		return ;
	}
}

void	get_redir(char **segment, char *end, t_redir *redir)
{
	while (*segment < end)
	{
		if (redir && !(redir->error == ALL_OK))
		{
			redir = NULL;
		}
		skipwhitesp(segment, end);
		if (*segment == ft_strnstr(*segment, "<<", end - *segment))
			handle_heredoc(segment, end, redir);
		else if (*segment == ft_strnstr(*segment, ">>", end - *segment))
			handle_append(segment, end, redir);
		else if (**segment == '<')
			handle_input(segment, end, redir);
		else if (**segment == '>')
			handle_output(segment, end, redir);
		else
			return ;
	}
	return ;
}
