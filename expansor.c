
#include "minishell.h"

int	is_expansible(char *str)
{
	while (*str)
	{
		if (*str == '$')
			return (1);
		if (*str == 39 && ft_strchr(str + 1, 39))
			str = ft_strchr(str + 1, 39);
		str++;
	}
	return (0);
}

expandstr(char *str, void **garbaje, int position)
{
	char *tmp_str;

	tmp_str = str;
	while (*tmp_str)
	{
		if (*tmp_str == '$')
		
			
		if (*tmp_str == 39 && ft_strchr(tmp_str + 1, 39))
			tmp_str = ft_strchr(tmp_str + 1, 39);
		tmp_str++;
	}


}

void	expand_task(t_task *node)
{
		int	counter;
		int	i;

		counter = count_expansions(((t_task *)node));
		node->garbage = ft_calloc(counter + 1, sizeof(char *));
		if (node->garbage == NULL)
			assert(0);//GESTIONAR ERROR
		(node->garbage)[counter] == NULL;
		counter--; //los arrays tienen elemento 0
		if (is_expansible(node->cmd))
		{
			expandstr(node->cmd, node->garbage, counter);
			counter--;
		}
		if (is_expansible(node->redir.infoo))
		{
			expandstr(node->redir.infoo, node->garbage, counter);
			counter--;
		}
		if (is_expansible(node->redir.outfile))
		{
			expandstr(node->redir.outfile, node->garbage, counter);
			counter--;
		}
		i = 0;
		while ((node->argv)[i])
		{
			if (is_expansible((node->argv)[i]))
			{
				expandstr((node->argv)[i], node->garbage, counter);
				counter--;
			}
			i++;
		}
}

//¿seria viable fundir expansion y cecking?
int	expand_tree(t_tree *node)
{


	if (node->type == PIPE)
	{
		expand_tree(((t_pipe *)node)->left);
		expand_tree(((t_pipe *)node)->rigth);
	}
	else if (node->type == TASK)
	{
		expand_task(((t_task *)node));
		unwarp(node);//limpia comillas no anidadas en otras comillas 
	}
}
