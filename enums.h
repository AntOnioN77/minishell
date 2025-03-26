/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enums.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jperez-r <jperez-r@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:57:52 by antofern          #+#    #+#             */
/*   Updated: 2025/03/26 11:55:34 by jperez-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

enum	e_nodes
{
	NONE,
	PIPE,
	TASK,
};
// infile	<
// outfile	>
// heredoc	<<
// append	>>
enum	e_symbols
{
	none,
	infile,
	outfile,
	heredoc,
	append,
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
	INVALID_TYPE,
	TASK_IS_VOID,
	SYNTAX_ERROR,
	READLINE_FAIL,
	TMP_FILE_ERROR,
	LINE_TOO_LONG,
	child_error_handler_fail,
	FAIL_BUILTINS_EXE,
	CONTINUE,
	END_CYCLE,
	FINISH,
}	t_errors;
