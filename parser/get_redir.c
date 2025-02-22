/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-r <jperez-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:11:59 by antofern          #+#    #+#             */
/*   Updated: 2025/02/22 23:24:07 by jperez-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void handle_heredoc(char **segment, char *end, t_redir *redir)
{
	(*segment) += 2;
 	if (redir)
	{
		redir->insymbol = heredoc;
		signal(SIGINT, handle_sigint_heredoc);
		getpntword(segment, end, &(redir->infoo));
		if(!redir->infoo || *(redir->infoo) == '\0')
			redir->error = SYNTAX_ERROR;
		char *line;
        while ((line = readline("")) != NULL)
        {
            if (g_ctrlc == SIGINT)
            {
                free(line);
                //g_ctrlc = 0; // Restablecer la variable
                return; // Salir del heredoc
            }
            // Procesar la línea de entrada
            // Aquí puedes escribir la línea a un archivo temporal o manejarla como necesites
            // por ejemplo: write_to_tmp_file(line);
            free(line);
		}
		/*if (line == NULL && g_ctrlc == SIGINT)
        {
            redir->error = E_SIGINT; // Marcar el error de interrupción
            return;
        }*/
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
		{
			handle_heredoc(segment, end, redir);
			if(g_ctrlc == 2)
			{
				g_ctrlc = 0;
				return ;
			}
		}
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
