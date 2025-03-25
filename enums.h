/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enums.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 01:38:55 by fibo              #+#    #+#             */
/*   Updated: 2025/03/25 01:39:48 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef enum e_nodes
{
	NONE,
	LINE,//probablemente innecesario
	PIPE,
	REDIR,//probablemente innecesario
	TASK,
	SYNTAX
}	e_nodes;

typedef enum e_symbols
{
	none,
	infile,//<
	outfile,//>
	heredoc,//<<
	append//>>
} e_symbols;

typedef enum e_errors
{
	ALL_OK,
	IS_A_DIR = 126,
	COM_NOT_FOUND = 127,
	E_SIGINT = 130,
	E_SIGQUIT = 131,
	ERROR_MALLOC = 151, // evitamos solapar valores de errno, de esta manera
	//nuestras funciones pueden retornar tanto valores capturados de errno, como
	//nuestros propios casos de error
	READ_SIGINT,
	NO_PERMISSION,
	NO_EXIST,
	INVALID_TYPE,//solo para debug
	TASK_IS_VOID,
	SYNTAX_ERROR,
	READLINE_FAIL,//lo he cambiado por retornar errno (14feb)
	TMP_FILE_ERROR,
	LINE_TOO_LONG,
	child_error_handler_fail,   //SOLO ODEBUG BORRAR!!!!!!!!!!!!!!!!!
	FAIL_BUILTINS_EXE,			//SOLO ODEBUG BORRAR!!!!!!!!!!!!!!!!!
	CONTINUE,
	END_CYCLE,
	FINISH
} e_errors;