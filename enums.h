/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enums.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:57:52 by antofern          #+#    #+#             */
/*   Updated: 2025/03/25 13:41:14 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

enum	e_nodes
{
	NONE,
//	LINE,//probablemente innecesario
	PIPE,
//	REDIR,//probablemente innecesario
	TASK,
//	SYNTAX //probablemente innecesario
};

enum	e_symbols
{
	none,
	infile,//<
	outfile,//>
	heredoc,//<<
	append,//>>
};

typedef enum e_errors
{
	ALL_OK,
	IS_A_DIR = 126,
	COM_NOT_FOUND = 127,
	E_SIGINT = 130,
	E_SIGQUIT = 131,
	ERROR_MALLOC = 151,
	READ_SIGINT,
	NO_PERMISSION,
	NO_EXIST,
	INVALID_TYPE,//solo para debug
	TASK_IS_VOID,
	SYNTAX_ERROR,
	READLINE_FAIL,//lo he cambiado por retornar errno (14feb)
	TMP_FILE_ERROR,
	LINE_TOO_LONG,
	child_error_handler_fail,//SOLO ODEBUG BORRAR!!!!!!!!!!!!!!!!!
	FAIL_BUILTINS_EXE,//SOLO ODEBUG BORRAR!!!!!!!!!!!!!!!!!
	CONTINUE,
	END_CYCLE,
	FINISH,
}	t_errors;
