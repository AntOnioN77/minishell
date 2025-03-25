/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_envp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 21:02:57 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/25 01:18:26 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>

char	*ft_getenv(const char *name, char *envp[])
{
	int		pos;
	char	*word_start;

	pos = search_var(envp, name);
	if (pos == -1)
		return (NULL);
	word_start = ft_strchr(envp[pos], '=') + 1;
	return (word_start);
}
