/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:11:59 by antofern          #+#    #+#             */
/*   Updated: 2025/03/05 13:38:30 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void handle_heredoc(char **segment, char *end, t_redir *redir)
{
	(*segment) += 2;
 	if (redir)
	{
		redir->insymbol = heredoc;
		getpntword(segment, end, &(redir->infoo));
		if(!redir->infoo || *(redir->infoo) == '\0')
			redir->error = SYNTAX_ERROR;
	}
	else
		getpntword(segment, end, NULL);
	return;
}

static void handle_append(char **segment, char *end, t_redir *redir)
{
	(*segment) += 2;
 	if (redir)
	{
		redir->outsymbol = append;
		getpntword(segment, end, &(redir->outfile));
	}
	else
		getpntword(segment, end, NULL);
}

static void handle_input(char **segment, char *end, t_redir *redir)
{
	(*segment)++;
 	if (redir)
	{
		redir->insymbol = infile;
		getpntword(segment, end, &(redir->infoo));
	}
	else
		getpntword(segment, end, NULL);
}

static void handle_output(char **segment, char *end, t_redir *redir)
{
{
	(*segment)++;
 	if (redir)
	{
		redir->outsymbol = outfile;
		getpntword(segment, end, &(redir->outfile));
	}
	else
		getpntword(segment, end, NULL);
}
}

//si lo primero que encuentra en segment es uno o varios redir los consume, avanzando segment. Si <redir> no es null, rellena las istancias consumidas
//Si hay un heredoc, crea el archivo temporal necesario.
void	get_redir(char **segment, char *end, t_redir *redir)
{
	while (*segment < end)
	{
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
			return;
	}
	return ;
}
