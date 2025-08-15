/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yatanagh <yatanagh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:36:20 by yatanagh          #+#    #+#             */
/*   Updated: 2024/11/15 15:52:03 by yatanagh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *dst, size_t n)
{
	size_t	i;
	size_t	j;

	if (!str && !n)
		return (NULL);
	if (!dst[0] && !n)
		return ((char *)str);
	if (!dst[0] && !str[0])
		return ((char *)str);
	i = 0;
	while (str[i] && i < n)
	{
		j = 0;
		while (dst[j] && str[i + j] && (str[i + j] == dst[j]) && (i + j) < n)
			j++;
		if (dst[j] == '\0')
			return ((char *)(str + i));
		if (i + j >= n)
			return (NULL);
		i++;
	}
	return (NULL);
}
