/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 14:03:36 by antofern          #+#    #+#             */
/*   Updated: 2024/08/26 13:33:19 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*current;
	void	*new_content;

	if (lst == NULL || f == NULL || del == NULL)
		return (NULL);
	new_content = (*f)(lst->content);
	new = ft_lstnew(new_content);
	if (new == NULL)
		return (NULL);
	current = new;
	lst = lst->next;
	while (lst != NULL)
	{
		new_content = (*f)(lst->content);
		current->next = ft_lstnew(new_content);
		if (current->next == NULL)
		{
			ft_lstclear(&new, del);
			return (NULL);
		}
		current = current->next;
		lst = lst->next;
	}
	return (new);
}
