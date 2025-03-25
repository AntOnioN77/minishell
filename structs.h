/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 01:43:53 by antofern          #+#    #+#             */
/*   Updated: 2025/03/26 00:31:26 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*_________________________TREE_STRUCTS______________________________________*/
typedef struct s_tree
{
	enum e_nodes	type;
	char			*line;
	char			*line_extra;
}	t_tree;

typedef struct s_redir
{
	enum e_nodes	type;
	enum e_symbols	insymbol;
	char			*infoo;
	enum e_symbols	outsymbol;
	char			*outfile;
	char			*tmp_file;
	t_errors		error;
}	t_redir;

typedef struct s_garbage
{
	void	**pointers;
	int		size;
	int		current;
}	t_garbage;

typedef struct s_task
{
	enum e_nodes	type;
	char			*line;
	char			*line_extra;
	t_redir			redir;
	char			*cmd;
	char			**argv;
	t_garbage		garb;
	int				pid;
}	t_task;

typedef struct s_pipe
{
	enum e_nodes	type;
	char			*line;
	char			*line_extra;
	t_task			*left;
	t_tree			*rigth;
}	t_pipe;

/*______________________________Other_Structs____________________________*/
typedef struct s_environ
{
	char	**envp;
	int		next;
	int		alloced;
}	t_environ;
