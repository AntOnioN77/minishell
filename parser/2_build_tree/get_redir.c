/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:11:59 by antofern          #+#    #+#             */
/*   Updated: 2025/03/14 21:23:11 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <errno.h>

char *findchars(char *str, char *end, char *wanted)
{
    char *ptr;
    
    if (!str || !end || !wanted)
        return end;
    
    // Recorremos la cadena desde str hasta end
    for (ptr = str; ptr < end; ptr++) {
        // Para cada carácter en la cadena destino, verificamos si está en wanted
        char *w;
        for (w = wanted; *w != '\0'; w++) {
            if (*ptr == *w) {
                // Encontrada coincidencia, devolvemos el puntero
                return ptr;
            }
        }
    }
    
    // No se encontró coincidencia, devolvemos end
    return end;
}

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

void create_file(char *segment, char *end, int flag, t_redir *redir)
{
	int fd;
	char *file;
	
	// Crear el archivo si no existe y si redir no es NULL (redir NULL significa que solo estamos usando get redir para skippear redirs)
	file = ft_substr(segment, 0, findchars(segment, end, DELIMITERS) - segment);
	if (redir && !access(file, F_OK) && access(file, W_OK) == -1)
		redir->error = NO_PERMISSION;
	else
	{
		fd = open(file, O_WRONLY | O_CREAT | flag, 0664);
		close(fd);
	}
	free(file);
}

static void handle_append(char **segment, char *end, t_redir *redir)
{
	int fd;
	char *file;

	
	(*segment) += 2;
	file = ft_substr(*segment, 0, findchars(*segment, end, DELIMITERS) - *segment);
	fd = open(file, O_APPEND, 0664);
	//		if (fd == -1)
	//			redir->error = errno;
	close(fd);
 	if (redir)
	{
		redir->outsymbol = append;
		getpntword(segment, end, &(redir->outfile));
	}
	else
		getpntword(segment, end, NULL);
	free(file);
	return;
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
		create_file(*segment, end, O_TRUNC, redir);

		// instanciar redir >
		redir->outsymbol = outfile;
		getpntword(segment, end, &(redir->outfile));
	}
	else
		getpntword(segment, end, NULL);
	return;
}
}

//si lo primero que encuentra en segment es uno o varios redir los consume, avanzando segment. Si <redir> no es null, rellena las istancias consumidas
//Si hay un heredoc, crea el archivo temporal necesario.
void	get_redir(char **segment, char *end, t_redir *redir)
{
//fprintf(stderr, "___________________________________get redir\n");
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
			return;
	}
	return ;
}
