/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:25:10 by antofern          #+#    #+#             */
/*   Updated: 2025/03/24 23:24:04 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H
# include "libft.h"

int		ft_has_duplicates(int n_words, char **array_words);
t_bool	ft_is_in_bounds_int(char *digits);
#endif