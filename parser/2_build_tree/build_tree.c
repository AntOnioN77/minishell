/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:19:13 by antofern          #+#    #+#             */
/*   Updated: 2025/02/24 12:55:02 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	getpntword(char **segment, char *end, char **dst)
{
	skipwhitesp(segment, end);
	if (dst != NULL)
		*dst = *segment;
	if (*segment == end && dst != NULL)
	{
		*dst = NULL;
		return ;
	}
	while (*segment < end)
	{
		skip_quotes(segment, end);
		if (!isdelimiter(**segment))
			(*segment)++;
		else
		{
			skipwhitesp(segment, end);
			return ;
		}
	}
	return ;
}

int	count_cmdflags(char *segment, char *end)
{
	int		i;
	char	*tmp;

	i = 0;
	while (segment < end)
	{
		get_redir(&segment, end, NULL);
		tmp = segment;
		getpntword(&segment, end, NULL);
		if (tmp != segment)
			i++;
	}
	return (i);
}

int	parse_task(char *segment, char *end, t_task *task)
{
	int	i;

	i = 0;
	while (segment < end)
	{
		get_redir(&segment, end, &(task->redir));
		if (!(segment < end))
			break ;
		if (!(task->cmd))
		{
			getpntword(&segment, end, &(task->cmd));
			task->argv[0] = task->cmd;
			i++;
		}
		else
		{
			getpntword(&segment, end, &(task->argv[i]));
			i++;
		}
	}
	task->argv[i] = NULL;
	return (0);
}

int	parsepipe(char *line, t_tree **ret)
{
	char	*pnt;

	pnt = line;
	if (!strnchr_outquot(&pnt, pnt + ft_strlen(pnt), '|'))
		return (0);
	*ret = createpipe(line, pnt);
	return (1);
}

t_tree	*build_tree(char *line)
{
	t_tree	*ret;

	if (0 == parsepipe(line, &ret))
		ret = (t_tree *)createtask(line, line + ft_strlen(line));
	nullify_delimiters(line);
	return (ret);
}
