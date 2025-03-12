
#include "../../minishell.h"
#include "../../libft/headers/libft.h"




static int	prepare_expansion(char **origin, char **new_str, char *envp[])
{
	int	len;

	len = calculate_expansion_length(*origin, envp);
	if (len < 0)
		return (1);
	*new_str = ft_calloc(len + 1, sizeof(char));
	if (*new_str == NULL)
	 	return (1);
//
	*origin = *new_str;
	(*new_str)[len] = '\0';
	return (0);
}


e_errors	expandstr(char **origin, char *envp[]) //envp debe recibir el array de strings que hemos creado y sobre el que se reflejan las modificaciones que pueda hacer minishell durante la ejecucion
{
	char	*marker;
	char	*new_str;
	char	*str;

	if(!is_expansible(*origin))
		return (ALL_OK);
	str = *origin;
	if (prepare_expansion(origin, &new_str, envp))
		return (ERROR_MALLOC);
	marker = str;
	while (*marker)
	{
		if (*marker == '$')
		{
			if(handle_dollar(&new_str, &str, &marker, envp))
				return (ERROR_MALLOC);
			continue ;
		}
		if (*marker == 39 && ft_strchr(marker + 1, 39))
			marker = ft_strchr(marker + 1, 39);
		marker++;
	}
	if (*str)
        ft_strlcpy(new_str, str, marker - str + 1);
    return (ALL_OK);
}

//
/*
static int	expand_task(t_task *node, char *envp[])
{
	int	i;

	node->garb.size = count_expansions(((t_task *)node)) + 1;
	node->garb.pointers = ft_calloc(node->garb.size, sizeof(void *));
	if (node->garb.pointers == NULL)
		return (1);
	(node->garb.pointers)[node->garb.size -1] = NULL;
	
	if (expandstr(&(node->cmd), envp))
		return (1);
//	if (add_pathname(&(node->cmd), &(node->garb), envp))
//		return (1);
	if (expandstr(&(node->redir.infoo), envp))
		return (1);
	if (expandstr(&(node->redir.outfile), envp))
		return (1);
	i = 0;
	while ((node->argv)[i])
	{
		if (expandstr((&(node->argv)[i]), envp))
			return (1);
		i++;
	}
	return(0);
}
*/

//verifica si es necesaria la expansion, si lo es, la realiza llamando a expandstr y libera la linea original
e_errors expansor(char **line, char **envp)
{
	char	*oldline;

	oldline = *line;
	if (is_expansible(*line))
	{
		oldline = *line;
		if (expandstr(line, envp))
		{
			free(oldline);
			return(ERROR_MALLOC);
		}
		free(oldline);
	}
	return(ALL_OK);
}
