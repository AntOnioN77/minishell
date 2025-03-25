/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_envp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 21:02:57 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/25 13:04:07 by antofern         ###   ########.fr       */
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
